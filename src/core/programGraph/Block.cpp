#include <error/InternalError.h>
#include <programGraph/Block.h>

Block::Block(size_t inputCount)
    : m_inputConnections(inputCount)
{
}

Block::~Block()
    = default;

size_t Block::inputCount()
{
    return inputTypes().size();
}

size_t Block::outputCount()
{
    return outputTypes().size();
}

std::vector<Connection> Block::inputConnections() const
{
    return m_inputConnections;
}

void Block::setInputConnections(std::vector<Connection> val)
{
    if (val.size() != inputCount()) {
        THROW_ERROR(InternalError, "Called Block::setInputConnections with incorrect number of Connections");
    }
    m_inputConnections = val;
}

void Block::setInputConnection(size_t index, Connection connection)
{
    m_inputConnections.at(index) = connection;
}
