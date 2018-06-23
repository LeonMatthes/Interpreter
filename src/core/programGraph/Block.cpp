#include <programGraph/Block.h>
#include <programGraph/Function.h>

Block::Block(Function& function)
	: m_function{function}
{
}

std::vector<Datatype> Block::inputTypes() const
{
	return function().inputs();
}

std::vector<Datatype> Block::outputTypes() const
{
	return function().outputs();
}

std::vector<Connection> Block::inputs() const
{
	return std::vector<Connection>();
}

Function& Block::function() const
{
	return m_function;
}
