#include <programGraph/GraphicalFunction.h>
#include <error/InternalError.h>
#include <utility/Literals.h>

GraphicalFunction::GraphicalFunction()
{}

GraphicalFunction::GraphicalFunction(std::vector<Datatype> inputs, std::vector<Datatype> outputs)
	: Function(inputs, outputs)
{}

GraphicalFunction::GraphicalFunction(GraphicalFunction&& other)
	: Function(other)
	, m_expressionBlocks(std::move(other.m_expressionBlocks))
	, m_statementBlocks(std::move(other.m_statementBlocks))
{}

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

void GraphicalFunction::addVariable(VariableIdentifier identifier, Datatype type)
{
	m_variables[identifier] = type;
}

Datatype GraphicalFunction::variableType(VariableIdentifier identifier) const
{
	if (!hasVariable(identifier))
	{
		THROW_ERROR(InternalError, "Tried to access variable type of variable: "_s + identifier + ", which does not exist!");
	}
	return m_variables.at(identifier);
}

bool GraphicalFunction::hasVariable(VariableIdentifier identifier) const
{
	return m_variables.find(identifier) != m_variables.end();
}

const std::unordered_map<VariableIdentifier, Datatype>& GraphicalFunction::variables() const
{
	return m_variables;
}
