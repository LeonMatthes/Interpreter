#include <programGraph/ExpressionStatement.h>
#include <programGraph/ExpressionBlock.h>

ExpressionStatement::ExpressionStatement(std::shared_ptr<class ExpressionBlock> expression)
	: StatementBlock(expression->outputCount())
	, m_expression(expression)
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
