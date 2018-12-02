#include <JSProgramTranslator.h>
#include <map>
#include <programGraph/GraphicalFunction.h>
#include <TranslationError.h>
#include <programGraph/ReturnBlock.h>
#include <JSProgram.h>
#include <visitors/TypeChecker.h>
#include <visitors/DAGChecker.h>
#include <programGraph/Value.h>
#include <programGraph/ValueBlock.h>
#include <programGraph/ProgramFlowConnection.h>
#include <programGraph/IfStatement.h>
#include <programGraph/WhileStatement.h>
#include <programGraph/ParameterAccessBlock.h>
#include <programGraph/ExpressionStatement.h>
#include <programGraph/PrimitiveFunction.h>
#include <utility/Literals.h>
#include <error/Error.h>
#include <programGraph/FunctionBlock.h>
#include <programGraph/VariableReadBlock.h>
#include <programGraph/VariableWriteBlock.h>

JSProgramTranslator::JSProgramTranslator()
{}

class JSProgram* JSProgramTranslator::translateProgram(v8::Local<v8::Object> javascriptProgram)
{
	auto jsFunctionsValue = Nan::Get(javascriptProgram, Nan::New("functions").ToLocalChecked()).ToLocalChecked();
	m_jsFunctionValues = translateMap(jsFunctionsValue, "Could not translate Program.functions to Map<Number, Object>");
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

	auto functionIdentifiers = functionIDs();

	auto startFunction = std::move(m_functions.at(startFunctionID));
	m_functions.erase(startFunctionID);

	auto functions = std::vector<GraphicalFunction::UPtr>();
	for (auto& function : m_functions)
	{
		functions.emplace_back(std::move(function.second));
	}
	functions.clear();

	return new JSProgram(std::move(startFunction), std::move(functions), functionIdentifiers, m_blockIdentifiers);
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
	if (!keys->IsObject())
	{
		throw TranslationError(errorMessage);
	}

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

std::map<JSProgramTranslator::Identifier, v8::Local<v8::Value>> JSProgramTranslator::translateMap(v8::Local<v8::Value> jsMapValue, std::string errorMessage)
{
	if (!jsMapValue->IsObject())
	{
		throw TranslationError(errorMessage);
	}
	auto jsMap = jsMapValue->ToObject();

	auto getFunctionValue = Nan::Get(jsMap, Nan::New("get").ToLocalChecked()).ToLocalChecked();
	if (!getFunctionValue->IsFunction())
	{
		throw TranslationError(errorMessage);
	}
	auto getFunction = v8::Handle<v8::Function>::Cast(getFunctionValue);

	auto keys = accessMapKeys(jsMap, errorMessage.data());
	auto result = std::map<Identifier, v8::Local<v8::Value>>();
	for (auto key : keys)
	{
		if (!key->IsNumber())
		{
			throw TranslationError(errorMessage);
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

	auto graphical = GraphicalFunction::UPtr(new GraphicalFunction(inputs, outputs));

	fillVariables(*graphical, ID, Nan::Get(jsFunction, Nan::New("variables").ToLocalChecked()).ToLocalChecked());

	return graphical;
}

auto JSProgramTranslator::functionIDs() ->std::map<Function*, Identifier>
{
	std::map<Function*, Identifier> identifiers;
	for (const auto& identifierFunctionPair : m_functions)
	{
		const auto& identifier = identifierFunctionPair.first;
		const auto& function = identifierFunctionPair.second;
		identifiers[function.get()] = identifier;
	}
	return identifiers;
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

	auto jsBlockValues = translateMap(Nan::Get(jsFunction, Nan::New("blocks").ToLocalChecked()).ToLocalChecked(), "Could not translate Function.blocks to Map<Number, Object>");
	auto blocksMap = translateBlockDeclarations(jsBlockValues, ID);

	auto jsConnections = Nan::Get(jsFunction, Nan::New("connections").ToLocalChecked()).ToLocalChecked();
	translateBlockConnections(jsConnections, blocksMap, ID);


	auto startBlockValue = Nan::Get(jsFunction, Nan::New("start").ToLocalChecked()).ToLocalChecked();
	if (!startBlockValue->IsNumber())
	{
		throw TranslationError("Function.start is not a number!", ID);
	}
	auto startBlock = Nan::To<int32_t>(startBlockValue).FromJust();


	if (blocksMap.count(startBlock) == 0)
	{
		throw TranslationError("Function.start is not a valid block Identifier!", ID);
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
}

std::map<JSProgramTranslator::Identifier, Block::Ptr> JSProgramTranslator::translateBlockDeclarations(std::map<Identifier, v8::Local<v8::Value>> jsBlockValues, Identifier currentFunctionID)
{
	auto blocks = std::map<Identifier, Block::Ptr>();
	for (const auto& blockIDJsBlockPair : jsBlockValues)
	{
		const auto& blockID = blockIDJsBlockPair.first;
		const auto& jsBlock = blockIDJsBlockPair.second;
		auto block = translateBlockDeclaration(Nan::Just<Identifier>(blockID), jsBlock, currentFunctionID);
		blocks[blockIDJsBlockPair.first] = block;
		m_blockIdentifiers[block.get()] = blockID;
	}

	return blocks;
}


Block::Ptr JSProgramTranslator::translateBlockDeclaration(Nan::Maybe<Identifier> ID, v8::Local<v8::Value> jsBlockValue, Identifier currentFunctionID)
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

	auto& currentFunction = *m_functions.at(currentFunctionID);
	//Expressions
	if (type->Equals(Nan::New("Value").ToLocalChecked()))
	{
		auto value = translateValue(Nan::Get(jsBlock, Nan::New("value").ToLocalChecked()).ToLocalChecked(), Nan::Just(currentFunctionID), ID);
		return std::make_shared<ValueBlock>(value);
	}
	else if (type->Equals(Nan::New("ParameterAccess").ToLocalChecked()))
	{
		return std::make_shared<ParameterAccessBlock>(currentFunction);
	}
	else if (type->Equals(Nan::New("Function").ToLocalChecked()))
	{
		return translateFunctionBlock(ID, jsBlock, currentFunctionID);
	}
	else if (type->Equals(Nan::New("VariableAccess").ToLocalChecked()))
	{
		auto variableValue = Nan::Get(jsBlock, Nan::New("variable").ToLocalChecked()).ToLocalChecked();
		if (!variableValue->IsString())
		{
			throw TranslationError("VariableAccessBlock.variable is not a String!", currentFunctionID, ID);
		}
	
		Nan::Utf8String identifierString(variableValue->ToString());
		auto length = identifierString.length();
		if (length <= 0)
		{
			throw TranslationError("Could not read VariableAccessBlock.variable as C++ string!\nThis is probably an internal conversion error!", currentFunctionID, ID);
		}

		auto identifier = VariableIdentifier(*identifierString, length);
		if (!currentFunction.hasVariable(identifier))
		{
			throw TranslationError("VariableAccessBlock can not access variable: '"_s + identifier + "' because it does not exist!", currentFunctionID, ID);
		}
		return std::make_shared<VariableReadBlock>(currentFunction, identifier);
	}
	else if (type->Equals(Nan::New("Return").ToLocalChecked())) //Statements
	{
		return std::make_shared<ReturnBlock>(currentFunction);
	}
	else if (type->Equals(Nan::New("If").ToLocalChecked()))
	{
		return std::make_shared<IfStatement>();
	}
	else if (type->Equals(Nan::New("While").ToLocalChecked()))
	{
		return std::make_shared<WhileStatement>();
	}
	else if (type->Equals(Nan::New("VariableWrite").ToLocalChecked()))
	{
		auto variableValue = Nan::Get(jsBlock, Nan::New("variable").ToLocalChecked()).ToLocalChecked();
		if (!variableValue->IsString())
		{
			throw TranslationError("VariableWriteBlock.variable is not a String!", currentFunctionID, ID);
		}

		Nan::Utf8String identifierString(variableValue->ToString());
		auto length = identifierString.length();
		if (length <= 0)
		{
			throw TranslationError("Could not read VariableWriteBlock.variable as C++ string!\nThis is probably an internal conversion error!", currentFunctionID, ID);
		}

		auto identifier = VariableIdentifier(*identifierString, length);
		if (!currentFunction.hasVariable(identifier))
		{
			throw TranslationError("VariableWriteBlock can not access variable: '"_s + identifier + "' because it does not exist!", currentFunctionID, ID);
		}
		return std::make_shared<VariableWriteBlock>(currentFunction, identifier);
	}
	else if (type->Equals(Nan::New("ExpressionStatement").ToLocalChecked()))
	{
		auto expressionValue = Nan::Get(jsBlock, Nan::New("expression").ToLocalChecked()).ToLocalChecked();
		auto expressionMaybe = translateBlockDeclaration(Nan::Nothing<Identifier>(), expressionValue, currentFunctionID);
		auto expression = std::dynamic_pointer_cast<ExpressionBlock>(expressionMaybe);
		if (expression == nullptr)
		{
			throw TranslationError("ExpressionStatement.expression is not an Expression!", currentFunctionID, ID);
		}

		return std::make_shared<ExpressionStatement>(expression);
	}
	else
	{
		Nan::Utf8String identifierString(type);
		auto length = identifierString.length();
		if (length <= 0)
		{
			throw TranslationError("Unsupported type!\nCould not read type as string!", currentFunctionID, ID);
		}
		else
		{
			throw TranslationError("Unsupported type: '" + std::string(*identifierString, length) + "'!", currentFunctionID, ID);
		}
	}
}

Block::Ptr JSProgramTranslator::translateFunctionBlock(Nan::Maybe<Identifier> ID, v8::Local<v8::Object> jsBlock, Identifier currentFunctionID)
{
	auto identifierValue = Nan::Get(jsBlock, Nan::New("functionID").ToLocalChecked()).ToLocalChecked();
	if (!identifierValue->IsNumber())
	{
		throw TranslationError("FunctionBlock.functionID is not an Identifier!", currentFunctionID, ID);
	}
	auto functionID = Nan::To<Identifier>(identifierValue).FromJust();

	auto primitiveValue = Nan::Get(jsBlock, Nan::New("primitive").ToLocalChecked()).ToLocalChecked();
	if (!primitiveValue->IsBoolean())
	{
		throw TranslationError("FunctionBlock.primitive is not a Boolean!", currentFunctionID, ID);
	}

	if (Nan::To<bool>(primitiveValue).FromJust())
	{
		auto primitives = JSProgramTranslator::primitiveIdentifiers();
		if (primitives.count(functionID) == 0)
		{
			throw TranslationError("FunctionBlock.functionID does not reference a primitive function, even though FunctionBlock.primitive is true!", currentFunctionID, ID);
		}
		auto& primitive = primitives.at(functionID);
		return std::make_shared<FunctionBlock>(primitive);
	}
	else
	{
		if (m_functions.count(functionID) == 0)
		{
			throw TranslationError("FunctionBlock.functionID does not reference a user defined function, even though FunctionBlock.primitive is false!", currentFunctionID, ID);
		}
		auto& graphicalFunction = *m_functions.at(functionID);
		return std::make_shared<FunctionBlock>(graphicalFunction);
	}
}

void JSProgramTranslator::fillVariables(GraphicalFunction& graphical, Identifier ID, v8::Local<v8::Value> jsVariablesValue)
{
	if (!jsVariablesValue->IsArray())
	{
		throw TranslationError("Function.variables is not an array!", ID);
	}
	auto jsVariables = v8::Local<v8::Array>::Cast(jsVariablesValue);

	for (size_t i = 0; i < jsVariables->Length(); i++)
	{
		fillVariable(graphical, ID, jsVariables->Get(i));
	}
}

void JSProgramTranslator::fillVariable(GraphicalFunction& graphical, Identifier ID, v8::Local<v8::Value> variableValue)
{
	if (!variableValue->IsObject())
	{
		throw TranslationError("Variable is not an Object!", ID);
	}
	auto variable = variableValue->ToObject();

	auto identifier = Nan::Get(variable, Nan::New("ID").ToLocalChecked()).ToLocalChecked();
	if (!identifier->IsString())
	{
		throw TranslationError("Variable Identifier must be a string!");
	}
	Nan::Utf8String identifierString(identifier);
	auto length = identifierString.length();
	if (length <= 0)
	{
		throw TranslationError("Could not read Variable.identifier as C++ string!\nThis is probably an internal conversion error!", ID);
	}

	auto type = translateDatatype(Nan::Get(variable, Nan::New("type").ToLocalChecked()).ToLocalChecked(), ID);
	
	graphical.addVariable(VariableIdentifier(*identifierString, length), type);
}

void JSProgramTranslator::translateBlockConnections(v8::Local<v8::Value> jsConnectionsValue, std::map<JSProgramTranslator::Identifier, Block::Ptr>& blocksMap, Identifier currentFunction)
{
	if (!jsConnectionsValue->IsArray())
	{
		throw TranslationError("Function.connections must be an Array!");
	}
	auto jsConnections = v8::Local<v8::Array>::Cast(jsConnectionsValue);

	for (size_t i = 0; i < jsConnections->Length(); i++)
	{
		translateBlockConnection(jsConnections->Get(i), blocksMap, currentFunction);
	}
}

void JSProgramTranslator::translateBlockConnection(v8::Local<v8::Value> jsConnectionValue, std::map<JSProgramTranslator::Identifier, Block::Ptr>& blocksMap, JSProgramTranslator::Identifier currentFunction)
{
	if (!jsConnectionValue->IsObject())
	{
		throw TranslationError("Connection is not an Object!", currentFunction);
	}
	auto jsConnection = jsConnectionValue->ToObject();

	auto startBlockValue = Nan::Get(jsConnection, Nan::New("startBlock").ToLocalChecked()).ToLocalChecked();
	auto startBlockID = translateIdentifier(startBlockValue, "Connection.startBlock must be an Identifier!", currentFunction);

	auto startPortValue = Nan::Get(jsConnection, Nan::New("startPort").ToLocalChecked()).ToLocalChecked();
	auto startPort = translateIdentifier(startPortValue, "Connection.startPort must be a Number!", currentFunction);

	auto endBlockValue = Nan::Get(jsConnection, Nan::New("endBlock").ToLocalChecked()).ToLocalChecked();
	auto endBlockID = translateIdentifier(endBlockValue, "Connection.endBlock must be an Identifier!", currentFunction);

	auto endPortValue = Nan::Get(jsConnection, Nan::New("endPort").ToLocalChecked()).ToLocalChecked();
	auto endPort = translateIdentifier(endPortValue, "Connection.endPort must be a Number!", currentFunction);

	if (blocksMap.count(startBlockID) == 0)
	{
		throw TranslationError("Connection.startBlock does not reference a Block in this function!", currentFunction);
	}
	auto& startBlock = blocksMap.at(startBlockID);

	if (blocksMap.count(endBlockID) == 0)
	{
		throw TranslationError("Connection.endBlock does not reference a Block in this function", currentFunction);
	}
	auto& endBlock = blocksMap.at(endBlockID);


	if (startPort < 0)
	{
		throw TranslationError("Connection.startPort is negative!", currentFunction);
	}
	if (endPort < 0)
	{
		throw TranslationError("Connection.endPort is negative!", currentFunction);
	}

	auto startStatement = std::dynamic_pointer_cast<StatementBlock>(startBlock);
	auto endStatement = std::dynamic_pointer_cast<StatementBlock>(endBlock);
	if (startStatement != nullptr)
	{
		if (startPort < startStatement->flowConnectionsCount())
		{
			if (endStatement == nullptr)
			{
				throw TranslationError("Connection.startPort refers to a FlowConnection port, but Connection.endBlock is not a Statement!", currentFunction);
			}
			startStatement->setFlowConnection(startPort, ProgramFlowConnection(endStatement));
			return;
		}
		else
		{
			//subtract the ProgramFlow outputs, as these Connection types are not differentiated in JS
			startPort -= startStatement->flowConnectionsCount();
		}
	}

	if (endStatement != nullptr)
	{
		endPort -= 1; //subtract the one ProgramFlow input, as these Connection types are not differentiated in JS
	}

	if (startPort < 0 || startPort >= startBlock->outputCount())
	{
		throw TranslationError("Connection.startPort is out of bounds!", currentFunction);
	}
	if (endPort < 0 || endPort >= endBlock->inputCount())
	{
		throw TranslationError("Connection.endPort is out of bounds!", currentFunction);
	}

	endBlock->setInputConnection(endPort, Connection(startBlock, startPort));
}

Value JSProgramTranslator::translateValue(v8::Local<v8::Value> jsValueValue, Nan::Maybe<Identifier> currentFunction, Nan::Maybe<Identifier> currentBlock)
{
	if (jsValueValue->IsNumber())
	{
		return Value(Nan::To<double>(jsValueValue).FromJust());
	}
	else if (jsValueValue->IsBoolean())
	{
		return Value(Nan::To<bool>(jsValueValue).FromJust());
	}
	else
	{
		throw TranslationError("Value must be either Boolean or Number type!", currentFunction, currentBlock);
	}
}

JSProgramTranslator::Identifier JSProgramTranslator::translateIdentifier(v8::Local<v8::Value> numberValue, std::string errorMessage, Identifier currentFunction)
{
	if (!numberValue->IsNumber())
	{
		throw TranslationError(errorMessage, currentFunction);
	}
	return Nan::To<Identifier>(numberValue).FromJust();
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



v8::Local<v8::Array> JSProgramTranslator::datatypeArrayToJS(v8::Isolate* isolate, std::vector<Datatype> datatypes)
{
	auto jsArray = v8::Array::New(isolate, datatypes.size());
	for (size_t i = 0; i < datatypes.size(); i++)
	{
		auto jsType = Nan::New(""_s + datatypes.at(i)).ToLocalChecked();
		Nan::Set(jsArray, i, jsType);
	}
	return jsArray;
}

#include <Constants.h>

std::map<JSProgramTranslator::Identifier, PrimitiveFunction&> JSProgramTranslator::primitiveIdentifiers()
{
	return std::map<JSProgramTranslator::Identifier, PrimitiveFunction&>({
			{ Identifiers::ADD, PrimitiveFunction::add },
			{ Identifiers::SUBTRACT, PrimitiveFunction::subtract },
			{ Identifiers::MULTIPLY, PrimitiveFunction::multiply },
			{ Identifiers::DIVIDE, PrimitiveFunction::divide },
			{ Identifiers::SMALLER, PrimitiveFunction::smaller },
			{ Identifiers::LOGICALAND, PrimitiveFunction::logicalAnd },
			{ Identifiers::LOGICALOR, PrimitiveFunction::logicalOr },
			{ Identifiers::LOGICALNOT, PrimitiveFunction::logicalNot},
			{ Identifiers::BOOLTODOUBLE, PrimitiveFunction::castBoolToDouble },
			{ Identifiers::DOUBLETOBOOL, PrimitiveFunction::castDoubleToBool }
		});
}

std::map<Datatype, JSProgramTranslator::Identifier> JSProgramTranslator::castingTable(Datatype type)
{
	switch (type)
	{
	case Datatype::DOUBLE:
		return { { Datatype::BOOLEAN, Identifiers::DOUBLETOBOOL } };
		break;
	case Datatype::BOOLEAN:
		return { {Datatype::DOUBLE, Identifiers::BOOLTODOUBLE} };
		break;
	}
	return {};
}

v8::Local<v8::Value> JSProgramTranslator::datatypeInformation(v8::Isolate* isolate, Datatype type)
{
	auto casts = castingTable(type);
	auto jsCastingTable = v8::Array::New(isolate, casts.size());
	
	auto i = 0;
	for (const auto& cast : casts)
	{
		auto jsCast = v8::Object::New(isolate);
		Nan::Set(jsCast, Nan::New("target").ToLocalChecked(), Nan::New(""_s + cast.first).ToLocalChecked());
		Nan::Set(jsCast, Nan::New("functionID").ToLocalChecked(), Nan::New(cast.second));

		Nan::Set(jsCastingTable, i++, jsCast);
	}
	
	auto jsDatatype = v8::Object::New(isolate);
	Nan::Set(jsDatatype, Nan::New("name").ToLocalChecked(), Nan::New(""_s + type).ToLocalChecked());
	Nan::Set(jsDatatype, Nan::New("casts").ToLocalChecked(), jsCastingTable);

	return jsDatatype;
}

NAN_METHOD(JSProgramTranslator::Primitives)
{
	try
	{
		auto identifiers = JSProgramTranslator::primitiveIdentifiers();
		auto primitivesArray = v8::Array::New(info.GetIsolate(), identifiers.size());
		auto count = 0;

		for (const auto& identifier : identifiers)
		{
			auto& primitiveFunction = identifier.second;

			auto primitiveObject = v8::Object::New(info.GetIsolate());
			Nan::Set(primitiveObject, Nan::New("ID").ToLocalChecked(), Nan::New(identifier.first));
			Nan::Set(primitiveObject, Nan::New("inputs").ToLocalChecked(), JSProgramTranslator::datatypeArrayToJS(info.GetIsolate(), primitiveFunction.inputs()));
			Nan::Set(primitiveObject, Nan::New("outputs").ToLocalChecked(), JSProgramTranslator::datatypeArrayToJS(info.GetIsolate(), primitiveFunction.outputs()));
			Nan::Set(primitiveObject, Nan::New("name").ToLocalChecked(), Nan::New(primitiveFunction.name()).ToLocalChecked());
		
			Nan::Set(primitivesArray, count++, primitiveObject);
		}

		info.GetReturnValue().Set(primitivesArray);
	}
	catch (std::exception& e)
	{
		TranslationError(e.what());
	}
	catch (Error::Ptr e)
	{
		TranslationError(e->message());
	}
	catch (TranslationError e)
	{
		;
	}
	catch (...)
	{
		TranslationError("Unknown C++ exception thrown!");
	}
}

NAN_METHOD(JSProgramTranslator::Datatypes)
{
	try
	{
		auto types = std::vector<Datatype>({ 
			Datatype::BOOLEAN, 
			Datatype::DOUBLE });

		auto jsArray = v8::Array::New(info.GetIsolate(), types.size());
		for (size_t i = 0; i < types.size(); i++)
		{
			Nan::Set(jsArray, i, datatypeInformation(info.GetIsolate(), types.at(i)));
		}
		info.GetReturnValue().Set(jsArray);
	}
	catch (std::exception& e)
	{
		TranslationError(e.what());
	}
	catch (...)
	{
		TranslationError("Unknown C++ error thrown!");
	}
}