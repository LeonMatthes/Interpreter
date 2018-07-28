#include <error/RuntimeError.h>
#include <utility/Literals.h>

RuntimeError::RuntimeError(const std::string& message)
	: Error(message)
{}

RuntimeError::~RuntimeError()
{}

std::string RuntimeError::message()
{
	return "Runtime Error: "_s + Error::message();
}

