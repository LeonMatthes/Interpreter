#include <programGraph/StatementBlock.h>
#include <error/InternalError.h>

StatementBlock::StatementBlock(size_t inputCount, size_t flowConnectionsCount /*= 1*/)
	: Block(inputCount)
	, m_flowConnections(flowConnectionsCount)
{}

StatementBlock::~StatementBlock()
{}

size_t StatementBlock::flowConnectionsCount()
{
	return m_flowConnections.size();
}

const std::vector<ProgramFlowConnection>& StatementBlock::flowConnections()
{
	return m_flowConnections;
}

void StatementBlock::setFlowConnections(const std::vector<ProgramFlowConnection>& connections)
{
	if (flowConnectionsCount() != connections.size())
	{
		THROW_ERROR(InternalError, "Invalid number of flowConnections!");
	}
	m_flowConnections = connections;
}

void StatementBlock::setFlowConnection(size_t index, const ProgramFlowConnection& connection)
{
	m_flowConnections.at(index) = connection;
}
