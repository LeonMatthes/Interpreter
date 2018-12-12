#include <programGraph/ExpressionBlock.h>
#include <programGraph/ExpressionStatement.h>

#include <utility>

#include <utility>

ExpressionStatement::ExpressionStatement(std::shared_ptr<class ExpressionBlock> expression)
	: StatementBlock(expression->outputCount(), 1)
	, m_expression(std::move(std::move(expression)))
{}

ExpressionStatement::~ExpressionStatement()
= default;

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
