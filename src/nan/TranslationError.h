#pragma once
#include <string>
#include <nan.h>
#include <JSProgramTranslator.h>

class TranslationError
{
public:
	TranslationError(std::string errorMessage);
	TranslationError(std::string errorMessage, JSProgramTranslator::Identifier functionIdentifier);
	TranslationError(std::string errorMessage, JSProgramTranslator::Identifier functionIdentifier, JSProgramTranslator::Identifier blockIdentifier);
	TranslationError(std::string errorMessage, JSProgramTranslator::Identifier functionIdentifier, Nan::Maybe<JSProgramTranslator::Identifier> blockIdentifier);



	virtual ~TranslationError() = default;
protected:
	
	void throwJSError(std::string errorMessage, JSProgramTranslator::Identifier functionIdentifier);
	void throwJSError(std::string errorMessage, JSProgramTranslator::Identifier functionIdentifier, JSProgramTranslator::Identifier blockIdentifier);
	void throwJSError(std::string message, v8::Local<v8::Value> functionIdentifier, v8::Local<v8::Value> blockIdentifier);
private:
};