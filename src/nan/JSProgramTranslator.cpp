#include <JSProgramTranslator.h>
#include <map>
#include <programGraph/GraphicalFunction.h>
#include <TranslationError.h>
#include <programGraph/ReturnBlock.h>
#include <JSProgram.h>
#include <visitors/TypeChecker.h>
#include <visitors/DAGChecker.h>

JSProgramTranslator::JSProgramTranslator()
{}

class JSProgram* JSProgramTranslator::translateProgram(v8::Local<v8::Object> javascriptProgram)
{
	auto jsFunctionsValue = Nan::Get(javascriptProgram, Nan::New("functions").ToLocalChecked()).ToLocalChecked();
	m_jsFunctionValues = translateMap(jsFunctionsValue);
	m_functions = std::move(translateFunctionDeclarations(m_jsFunctionValues));
	fillFunctionDefinitions(m_jsFunctionValues);
	

	auto startFunctionIDValue = Nan::Get(javascriptProgram, Nan::New("start").ToLocalChecked()).ToLocalChecked();
	if (!startFunctionIDValue->IsNumber())
	{
		throw TranslationError("Program.start is not a number!");
	}
	auto startFunctionID = Nan::To<int32_t>(startFunctionIDValue).FromJust();
	
	if (m_functions.count(startFunctionID) == 0)
	{
		throw TranslationError("Program.start is not a valid function identifier!");
	}
	auto startFunction = std::move(m_functions.at(startFunctionID));
	m_functions.erase(startFunctionID);

	auto functions = std::vector<GraphicalFunction::UPtr>();
	for (auto& function : m_functions)
	{
		functions.emplace_back(std::move(function.second));
	}
	functions.clear();

	return new JSProgram(std::move(startFunction), std::move(functions));
}

std::map<JSProgramTranslator::Identifier, v8::Local<v8::Value>> JSProgramTranslator::translateMap(v8::Local<v8::Value> jsMapValue)
{
	if (!jsMapValue->IsObject())
	{
		throw TranslationError("Tried to translate a Map, but it does not seem to be an Object!\nExpected: Map<Number, Object>");
	}
	auto jsMap = jsMapValue->ToObject();

	auto getFunctionValue = Nan::Get(jsMap, Nan::New("get").ToLocalChecked()).ToLocalChecked();
	if (!getFunctionValue->IsFunction())
	{
		throw TranslationError("Tried to convert a Map, but it doesn't understand 'get'!");
	}
	auto getFunction = v8::Handle<v8::Function>::Cast(getFunctionValue);

	auto keys = accessMapKeys(jsMap, "Tried to convert a Map, but it does not contain keys!");
	auto result = std::map<Identifier, v8::Local<v8::Value>>();
	for (auto key : keys)
	{
		if (!key->IsNumber())
		{
			throw TranslationError("One of the keys in a Map is not a number!");
		}
		auto keyNumber = Nan::To<Identifier>(key).FromJust();

		result[keyNumber] = getFunction->Call(jsMap, 1, &key);
	}
	return result;
}

std::map<JSProgramTranslator::Identifier, GraphicalFunction::UPtr> JSProgramTranslator::translateFunctionDeclarations(std::map<Identifier, v8::Local<v8::Value>> jsFunctionsMap)
{
	auto result = std::map<Identifier, GraphicalFunction::UPtr>();
	for (auto mapEntry : jsFunctionsMap)
	{
		result[mapEntry.first] = translateFunctionDeclaration(mapEntry.first, mapEntry.second);
	}
	return result;
}

GraphicalFunction::UPtr JSProgramTranslator::translateFunctionDeclaration(Identifier ID, v8::Local<v8::Value> jsFunctionValue)
{
	if (!jsFunctionValue->IsObject())
	{
		throw TranslationError("Function is not an object!", ID);
	}
	auto jsFunction = jsFunctionValue->ToObject();

	//#TODO variables

	auto inputs = translateDatatypeArray(Nan::Get(jsFunction, Nan::New("inputs").ToLocalChecked()).ToLocalChecked(), "Function.inputs is not an Array!", ID);
	auto outputs = translateDatatypeArray(Nan::Get(jsFunction, Nan::New("outputs").ToLocalChecked()).ToLocalChecked(), "Function.outputs is not an Array!", ID);

	return GraphicalFunction::UPtr(new GraphicalFunction(inputs, outputs));
}

void JSProgramTranslator::fillFunctionDefinitions(std::map<Identifier, v8::Local<v8::Value>>& jsDeclarations)
{
	for (auto& declaration : m_functions)
	{
		fillFunctionDefinition(declaration.first, jsDeclarations.at(declaration.first));
	}
}

void JSProgramTranslator::fillFunctionDefinition(Identifier ID, v8::Local<v8::Value> jsFunctionValue)
{
	if (!jsFunctionValue->IsObject())
	{
		throw TranslationError("Function is not an object!", ID);
	}
	auto jsFunction = jsFunctionValue->ToObject();

	auto jsBlockValues = translateMap(Nan::Get(jsFunction, Nan::New("blocks").ToLocalChecked()).ToLocalChecked());
	auto blocksMap = translateBlockDeclarations(jsBlockValues, ID);

	auto startBlockValue = Nan::Get(jsFunction, Nan::New("start").ToLocalChecked()).ToLocalChecked();
	if (!startBlockValue->IsNumber())
	{
		throw TranslationError("Function.start is not a number!", ID);
	}
	auto startBlock = Nan::To<int32_t>(startBlockValue).FromJust();

	
	if (blocksMap.count(startBlock) == 0)
	{
		throw TranslationError("Function.start is not a valid Identifier!", ID);
	}

	auto statementsList = std::vector<StatementBlock::Ptr>();
	auto expressionsList = std::vector<ExpressionBlock::Ptr>();
	if (auto startStatement = std::dynamic_pointer_cast<StatementBlock>(blocksMap.at(startBlock)))
	{
		statementsList.emplace_back(startStatement);
		blocksMap.erase(startBlock);
	}
	else
	{
		throw TranslationError("Function.start refers to a Block which is not a statement!", ID);
	}

	for (const auto& block : blocksMap)
	{
		if (auto statement = std::dynamic_pointer_cast<StatementBlock>(block.second))
		{
			statementsList.emplace_back(statement);
		}
		else if (auto expression = std::dynamic_pointer_cast<ExpressionBlock>(block.second))
		{
			expressionsList.emplace_back(expression);
		}
		else
		{
			throw TranslationError("One of the Blocks seems to be neither statement nor expression!", ID);
		}
	}
	auto& graphical = m_functions.at(ID);
	graphical->setStatementBlocks(statementsList);
	graphical->setExpressionBlocks(expressionsList);

	checkFunction(ID);
}

void JSProgramTranslator::checkFunction(Identifier ID)
{
	auto& function = *m_functions.at(ID);

	auto dagChecker = DAGChecker();
	if (!dagChecker.check(function))
	{
		throw TranslationError("Function did not pass DAG check!", ID);
	}
	
	auto typeChecker = TypeChecker();
	if (!function.accept(typeChecker))
	{
		throw TranslationError("Function did not pass type check!", ID);
	}

}

std::map<JSProgramTranslator::Identifier, Block::Ptr> JSProgramTranslator::translateBlockDeclarations(std::map<Identifier, v8::Local<v8::Value>> jsBlockValues, Identifier currentFunctionID)
{
	auto blocks = std::map<Identifier, Block::Ptr>();
	for (const auto& block : jsBlockValues)
	{
		blocks[block.first] = translateBlockDeclaration(block.first, block.second, currentFunctionID);
	}

	return blocks;
}


Block::Ptr JSProgramTranslator::translateBlockDeclaration(Identifier ID, v8::Local<v8::Value> jsBlockValue, Identifier currentFunctionID)
{
	if (!jsBlockValue->IsObject())
	{
		throw TranslationError("Block is not an object!", currentFunctionID, ID);
	}
	auto jsBlock = jsBlockValue->ToObject();
	
	auto typeValue = Nan::Get(jsBlock, Nan::New("type").ToLocalChecked()).ToLocalChecked();
	if (!typeValue->IsString())
	{
		throw TranslationError("Block.type is not a string!", currentFunctionID, ID);
	}
	auto type = typeValue->ToString();

	if (type->Equals(Nan::New("Return").ToLocalChecked()))
	{
		return std::make_shared<ReturnBlock>(*m_functions.at(currentFunctionID));
	}
	else
	{
		Nan::Utf8String typeString(type);
		auto length = typeString.length();
		if (length <= 0)
		{
			throw TranslationError("Unsupported type!\nCould not read type as string!", currentFunctionID, ID);
		}
		else
		{
			throw TranslationError("Unsupported type: '" + std::string(*typeString, length) + "'!", currentFunctionID, ID);
		}
	}
}

std::vector<Datatype> JSProgramTranslator::translateDatatypeArray(v8::Local<v8::Value> jsArrayValue, std::string errorMessage, Identifier currentFunctionID)
{
	if (!jsArrayValue->IsArray())
	{
		throw TranslationError(errorMessage, currentFunctionID);
	}
	
	auto jsArray = v8::Local<v8::Array>::Cast(jsArrayValue);
	auto types = std::vector<Datatype>();
	for (uint32_t i = 0; i < jsArray->Length(); i++)
	{
		types.emplace_back(translateDatatype(jsArray->Get(i), currentFunctionID));
	}
	return types;
}

Datatype JSProgramTranslator::translateDatatype(v8::Local<v8::Value> jsDatatype, Identifier currentFunctionID)
{
	if (!jsDatatype->IsString())
	{
		throw TranslationError("Tried to convert Datatype, but provided value is not a String!", currentFunctionID);
	}

	auto typeName = jsDatatype->ToString();
	if (typeName->Equals(Nan::New("Double").ToLocalChecked()))
	{
		return Datatype::DOUBLE;
	}
	else if (typeName->Equals(Nan::New("Boolean").ToLocalChecked()))
	{
		return Datatype::BOOLEAN;
	}
	else
	{
		throw TranslationError("Datatype name not supported!\nSupported types: 'Double', 'Boolean'!", currentFunctionID);
	}
}

std::vector<v8::Local<v8::Value>> JSProgramTranslator::accessMapKeys(v8::Local<v8::Object> javascriptMap, const char* errorMessage)
{
	auto keysFunctionValue = Nan::Get(javascriptMap, Nan::New("keys").ToLocalChecked()).ToLocalChecked();
	if (!keysFunctionValue->IsFunction())
	{
		throw TranslationError(errorMessage);
	}
	auto keysFunction = v8::Handle<v8::Function>::Cast(keysFunctionValue);
	auto keys = keysFunction->Call(javascriptMap, 0, nullptr);
	
	auto arrayPrototype = Nan::To<v8::Object>(v8::Isolate::GetCurrent()->GetCurrentContext()->Global()->Get(Nan::New("Array").ToLocalChecked())).ToLocalChecked();
	auto fromFunctionValue = Nan::Get(arrayPrototype, Nan::New("from").ToLocalChecked()).ToLocalChecked();
	if (!fromFunctionValue->IsFunction())
	{
		throw TranslationError(errorMessage);
	}
	auto fromFunction = v8::Handle<v8::Function>::Cast(fromFunctionValue);

	auto keysArrayValue = fromFunction->Call(arrayPrototype, 1, &keys);
	if (!keysArrayValue->IsArray())
	{
		throw TranslationError(errorMessage);
	}
	auto keysArray = v8::Local<v8::Array>::Cast(keysArrayValue);

	auto result = std::vector<v8::Local<v8::Value>>();
	for (uint32_t i = 0; i < keysArray->Length(); i++)
	{
		result.push_back(keysArray->Get(i));
	}

	return result;
}
