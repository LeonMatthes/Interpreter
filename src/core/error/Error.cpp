#include <error/Error.h>

#include <utility>

Error::Error(std::string  message)
	: m_message(std::move(message))
{
}

std::string Error::message() const
{
	return m_message;
}
