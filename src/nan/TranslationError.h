#pragma once
#include <string>
#include <nan.h>

class TranslationError
{
public:
	TranslationError(std::string errorMessage);
	virtual ~TranslationError() = default;
protected:
	
private:
};