#pragma once

#include <error/Error.h>

class InternalError : public Error
{
public:
	InternalError(std::string message);
	virtual ~InternalError() = default;

	virtual std::string message() const override;

	static const std::string prefix;
protected:
	
private:
};