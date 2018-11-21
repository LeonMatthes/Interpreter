#include <error/Error.h>

Error::Error(std::string message)
	: m_message(message)
{
}

std::string Error::message() const
{
	return m_message;
}
