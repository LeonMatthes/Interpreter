#pragma once
#include <error/Error.h>

class RuntimeError : public Error
{
public:
	using Ptr = std::shared_ptr<RuntimeError>;

	RuntimeError(const std::string& message, class Function* function = nullptr, class Block* block = nullptr);
	virtual ~RuntimeError();

	virtual std::string message() const override;
	class Function* function() const;
	class Block* block() const;
protected:
	
	class Function* m_function;
	class Block* m_Block;
private:
};