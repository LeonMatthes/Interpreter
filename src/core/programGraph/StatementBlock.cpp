#include <programGraph/StatementBlock.h>


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
