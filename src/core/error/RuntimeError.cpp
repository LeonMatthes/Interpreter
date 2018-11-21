#include <error/RuntimeError.h>
#include <utility/Literals.h>

RuntimeError::RuntimeError(const std::string& message, class Function* function /*= nullptr*/, class Block* block /*= nullptr*/)
	: Error(message)
	, m_function(function)
	, m_Block(block)
{}

RuntimeError::~RuntimeError()
{}

std::string RuntimeError::message() const
{
	return "Runtime Error: "_s + Error::message();
}

class Function* RuntimeError::function() const
{
	return m_function;
}

class Block* RuntimeError::block() const
{
	return m_Block;
}

