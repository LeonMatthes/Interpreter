#include <TranslationError.h>

TranslationError::TranslationError(std::string errorMessage)
{
	throwJSError(errorMessage, Nan::Undefined(), Nan::Undefined());
}

TranslationError::TranslationError(std::string errorMessage, JSProgramTranslator::Identifier functionIdentifier)
{
	auto message = "Failed to translate function: '" + std::to_string(functionIdentifier) + "'!\n";
	message += errorMessage;
	throwJSError(message, Nan::New(static_cast<int32_t>(functionIdentifier)), Nan::Undefined());
}

TranslationError::TranslationError(std::string errorMessage, JSProgramTranslator::Identifier functionIdentifier, JSProgramTranslator::Identifier blockIdentifier)
{
	auto message = "Failed to translate block: '" + std::to_string(blockIdentifier) + "' in function: '" + std::to_string(functionIdentifier) + "'!\n";
	message += errorMessage;
	throwJSError(message, Nan::New(static_cast<int32_t>(functionIdentifier)), Nan::New(static_cast<int32_t>(blockIdentifier)));
}

void TranslationError::throwJSError(std::string message, v8::Local<v8::Value> functionIdentifier, v8::Local<v8::Value> blockIdentifier)
{
	auto errorObject = Nan::Error(("TranslationError: " + message).data());
	Nan::Set(errorObject->ToObject(), Nan::New("functionID").ToLocalChecked(), functionIdentifier);
	Nan::Set(errorObject->ToObject(), Nan::New("blockID").ToLocalChecked(), blockIdentifier);
	Nan::ThrowError(errorObject);
}
