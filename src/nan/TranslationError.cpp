#include <TranslationError.h>

TranslationError::TranslationError(std::string errorMessage)
{
	Nan::ThrowError((std::string("TranslationError:") + errorMessage).data());
}