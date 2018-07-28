#pragma once
#include <error/Error.h>

class RuntimeError : public Error
{
public:
	RuntimeError(const std::string& message);
	virtual ~RuntimeError();

	virtual std::string message() override;
protected:
	
private:
};