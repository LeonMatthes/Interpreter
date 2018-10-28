#include <error/InternalError.h>

InternalError::InternalError(std::string message)
	: Error(message)
{}

std::string InternalError::message()
{
	return InternalError::prefix + Error::message();
}

const std::string InternalError::prefix = std::string("Internal Error: ");
