#pragma once

#include <error/Error.h>

class InternalError : public Error
{
public:
	explicit InternalError(std::string message);
	~InternalError() override = default;

	std::string message() const override;

	static const std::string prefix;
protected:
	
private:
};