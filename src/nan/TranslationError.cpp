#include <TranslationError.h>

TranslationError::TranslationError(std::string errorMessage)
{
	throwJSError(errorMessage, Nan::Undefined(), Nan::Undefined());
}

TranslationError::TranslationError(std::string errorMessage, JSProgramTranslator::Identifier functionIdentifier)
{
	throwJSError(errorMessage, functionIdentifier);

}

TranslationError::TranslationError(std::string errorMessage, JSProgramTranslator::Identifier functionIdentifier, JSProgramTranslator::Identifier blockIdentifier)
{
	throwJSError(errorMessage, functionIdentifier, blockIdentifier);

}

TranslationError::TranslationError(std::string errorMessage, JSProgramTranslator::Identifier functionIdentifier, Nan::Maybe<JSProgramTranslator::Identifier> blockIdentifier)
{
	if (blockIdentifier.IsNothing())
	{
		throwJSError(errorMessage, functionIdentifier);
	}
	else
	{
		throwJSError(errorMessage, functionIdentifier, blockIdentifier.FromJust());
	}
}

TranslationError::TranslationError(std::string errorMessage, Nan::Maybe<JSProgramTranslator::Identifier> functionIdentifier, Nan::Maybe<JSProgramTranslator::Identifier> blockIdentifier)
{
	if (functionIdentifier.IsNothing())
	{
		throwJSError(errorMessage, Nan::Undefined(), Nan::Undefined());
	}
	else if (blockIdentifier.IsNothing())
	{
		throwJSError(errorMessage, functionIdentifier.FromJust());
	}
	else
	{
		throwJSError(errorMessage, functionIdentifier.FromJust(), blockIdentifier.FromJust());
	}
}

void TranslationError::throwJSError(std::string message, v8::Local<v8::Value> functionIdentifier, v8::Local<v8::Value> blockIdentifier)
{
	auto errorObject = Nan::Error(("TranslationError: " + message).data());
	Nan::Set(errorObject->ToObject(), Nan::New("functionID").ToLocalChecked(), functionIdentifier);
	Nan::Set(errorObject->ToObject(), Nan::New("blockID").ToLocalChecked(), blockIdentifier);
	Nan::ThrowError(errorObject);
}

void TranslationError::throwJSError(std::string errorMessage, JSProgramTranslator::Identifier functionIdentifier)
{
	auto message = "Failed to translate function: '" + std::to_string(functionIdentifier) + "'!\n";
	message += errorMessage;
	throwJSError(message, Nan::New(static_cast<int32_t>(functionIdentifier)), Nan::Undefined());
}

void TranslationError::throwJSError(std::string errorMessage, JSProgramTranslator::Identifier functionIdentifier, JSProgramTranslator::Identifier blockIdentifier)
{
	auto message = "Failed to translate block: '" + std::to_string(blockIdentifier) + "' in function: '" + std::to_string(functionIdentifier) + "'!\n";
	message += errorMessage;
	throwJSError(message, Nan::New(static_cast<int32_t>(functionIdentifier)), Nan::New(static_cast<int32_t>(blockIdentifier)));
}
