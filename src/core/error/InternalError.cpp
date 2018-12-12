#include <error/InternalError.h>

#include <utility>

InternalError::InternalError(std::string message)
    : Error(std::move(message))
{
}

std::string InternalError::message() const
{
    return InternalError::prefix + Error::message();
}

const std::string InternalError::prefix = std::string("Internal Error: ");
