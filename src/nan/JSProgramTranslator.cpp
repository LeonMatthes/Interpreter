#include <JSProgramTranslator.h>
#include <map>
#include <programGraph/GraphicalFunction.h>
#include <TranslationError.h>

JSProgramTranslator::JSProgramTranslator()
{}

class JSProgram* JSProgramTranslator::translateProgram(v8::Local<v8::Object> javascriptProgram)
{
	auto jsFunctions = Nan::Get(javascriptProgram, Nan::New("functions").ToLocalChecked()).ToLocalChecked();
	auto functions = translateFunctionDeclarations(jsFunctions);
	return nullptr;
}

std::map<int64_t, GraphicalFunction::UPtr> JSProgramTranslator::translateFunctionDeclarations(v8::Local<v8::Value> functionMapValue)
{
	if (!functionMapValue->IsObject())
	{
		throw TranslationError("Attribute 'functions' in provided program is not an Object!\nExpected: Map<Number, Function>");
	}
	auto functionMap = functionMapValue->ToObject();

	auto getFunctionValue = Nan::Get(functionMap, Nan::New("get").ToLocalChecked()).ToLocalChecked();
	if (!getFunctionValue->IsFunction())
	{
		throw TranslationError("The 'functions' attribute in the provided program does not seem to be a Map!");
	}
	auto getFunction = v8::Handle<v8::Function>::Cast(getFunctionValue);

	auto keys = accessMapKeys(functionMap, "Could not access keys of 'function' attribute in provided program!");
	auto result = std::map<int64_t, GraphicalFunction::UPtr>();
	for (auto key : keys)
	{
		if (!key->IsNumber())
		{
			throw TranslationError("One of the keys in the 'functions' object is not a Number!");
		}
		auto keyNumber = Nan::To<int64_t>(key).FromJust();

		auto function = translateFunctionDeclaration(getFunction->Call(functionMap, 1, &key));
		result[keyNumber] = std::move(function);
	}
	return result;
}

GraphicalFunction::UPtr JSProgramTranslator::translateFunctionDeclaration(v8::Local<v8::Value> jsFunctionValue)
{
	if (!jsFunctionValue->IsObject())
	{
		throw TranslationError("Tried to translate function, but the provided value is not an object!");
	}
	auto jsFunction = jsFunctionValue->ToObject();

	auto inputs = translateDatatypeArray(Nan::Get(jsFunction, Nan::New("inputs").ToLocalChecked()).ToLocalChecked());
	auto outputs = translateDatatypeArray(Nan::Get(jsFunction, Nan::New("outputs").ToLocalChecked()).ToLocalChecked());

	return GraphicalFunction::UPtr(new GraphicalFunction(inputs, outputs));
}

std::vector<Datatype> JSProgramTranslator::translateDatatypeArray(v8::Local<v8::Value> jsArrayValue)
{
	if (!jsArrayValue->IsArray())
	{
		throw TranslationError("Expected array of types, but value is something different!");
	}
	
	auto jsArray = v8::Local<v8::Array>::Cast(jsArrayValue);
	auto types = std::vector<Datatype>(jsArray->Length());
	for (size_t i = 0; i < jsArray->Length(); i++)
	{
		types.emplace_back(translateDatatype(jsArray->Get(i)));
	}
	return types;
}

Datatype JSProgramTranslator::translateDatatype(v8::Local<v8::Value> jsDatatype)
{
	if (!jsDatatype->IsObject())
	{
		throw TranslationError("Tried to convert Datatype, but provided value is not a String!");
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
		throw TranslationError("Datatype name not supported!\nSupported types: 'Double', 'Boolean'!");
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
	for (size_t i = 0; i < keysArray->Length(); i++)
	{
		result.push_back(keysArray->Get(i));
	}

	return result;
}

class GraphicalFunction::UPtr JSProgramTranslator::translateFunction(v8::Local<v8::Object> javascriptFunction)
{
	return nullptr;
}

