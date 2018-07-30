#include <programGraph/GraphicalFunction.h>

GraphicalFunction::GraphicalFunction()
{}

GraphicalFunction::GraphicalFunction(std::vector<Datatype> inputs, std::vector<Datatype> outputs)
	: Function(inputs, outputs)
{}

GraphicalFunction::GraphicalFunction(GraphicalFunction&& other)
	: Function(other)
{
	m_expressionBlocks = std::move(other.m_expressionBlocks);
	m_statementBlocks = std::move(other.m_statementBlocks);
}

GraphicalFunction::~GraphicalFunction()
{}

const std::vector<ExpressionBlock::Ptr>& GraphicalFunction::expressionBlocks() const
{
	return m_expressionBlocks;
}

void GraphicalFunction::setExpressionBlocks(std::vector<ExpressionBlock::Ptr> blocks)
{
	m_expressionBlocks = blocks;
}

const std::vector<StatementBlock::Ptr>& GraphicalFunction::statementBlocks() const
{
	return m_statementBlocks;
}

void GraphicalFunction::setStatementBlocks(std::vector<StatementBlock::Ptr> blocks)
{
	m_statementBlocks = blocks;
}
