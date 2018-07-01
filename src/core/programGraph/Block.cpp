#include <programGraph/Block.h>
#include <error/InternalError.h>

Block::Block(size_t inputCount)
	: m_inputConnections(inputCount)
{
}

Block::~Block()
{}

size_t Block::inputCount()
{
	return inputTypes().size();
}


std::vector<Connection> Block::inputConnections() const
{
	return m_inputConnections;
}

void Block::setInputConnections(std::vector<Connection> val)
{
	if (val.size() != inputCount())
	{
		THROW_ERROR(InternalError, "Called Block::setInputConnections with incorrect number of Connections");
	}
	m_inputConnections = val;
}
