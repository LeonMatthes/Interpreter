#include <programGraph/ExpressionStatement.h>
#include <programGraph/ExpressionBlock.h>

ExpressionStatement::ExpressionStatement(std::unique_ptr<class ExpressionBlock> expression)
	: StatementBlock(expression->outputCount())
	, m_expression(std::move(expression))
{}

ExpressionStatement::~ExpressionStatement()
{

}

std::vector<Datatype> ExpressionStatement::inputTypes() const
{
	return m_expression->inputTypes();
}

std::vector<Datatype> ExpressionStatement::outputTypes() const
{
	return m_expression->outputTypes();
}

void ExpressionStatement::setInputConnections(std::vector<Connection> val)
{
	StatementBlock::setInputConnections(val);
	m_expression->setInputConnections(val);
}

ExpressionBlock& ExpressionStatement::expression()
{
	return *m_expression;
}
