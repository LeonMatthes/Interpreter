#include <programGraph/VariableReadBlock.h>
#include <error/InternalError.h>
#include <utility/Literals.h>

VariableReadBlock::VariableReadBlock(const GraphicalFunction& owningFunction, VariableIdentifier identifier) : ExpressionBlock(0)
	, m_owningFunction(owningFunction)
	, m_identifier(std::move(identifier))
{
	if (!m_owningFunction.hasVariable(m_identifier))
	{
		THROW_ERROR(InternalError, "Created VariableAccessBlock with idenitifer: "_s + identifier + " which does not exist in owning function!");
	}
}

VariableReadBlock::~VariableReadBlock()
{}

std::vector<Datatype> VariableReadBlock::inputTypes() const
{
	return {};
}

std::vector<Datatype> VariableReadBlock::outputTypes() const
{
	return { m_owningFunction.variableType(m_identifier) };
}

const VariableIdentifier& VariableReadBlock::variableIdentifier() const
{
	return m_identifier;
}

