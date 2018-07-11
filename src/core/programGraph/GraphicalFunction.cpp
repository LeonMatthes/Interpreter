#include <programGraph/GraphicalFunction.h>

GraphicalFunction::GraphicalFunction()
{}

GraphicalFunction::GraphicalFunction(std::vector<Datatype> inputs, std::vector<Datatype> outputs)
	: Function(inputs, outputs)
{}

GraphicalFunction::~GraphicalFunction()
{}

std::vector<ExpressionBlock::Ptr> GraphicalFunction::expressionBlocks()
{
	return m_expressionBlocks;
}

void GraphicalFunction::setExpressionBlocks(std::vector<ExpressionBlock::Ptr> blocks)
{
	m_expressionBlocks = blocks;
}

std::vector<StatementBlock::Ptr> GraphicalFunction::statementBlocks()
{
	return m_statementBlocks;
}

void GraphicalFunction::setStatementBlocks(std::vector<StatementBlock::Ptr> blocks)
{
	m_statementBlocks = blocks;
}
