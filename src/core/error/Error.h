#pragma once
#include <string>
#include <memory>

#define THROW_ERROR(ErrorClass, ...) throw std::dynamic_pointer_cast<Error>(std::make_shared<ErrorClass>(__VA_ARGS__));

class Error
{
public:
	typedef std::shared_ptr<Error> Ptr;

	Error(std::string message);
	virtual ~Error() = default;

	virtual std::string message();

protected:
	std::string m_message;
private:
};