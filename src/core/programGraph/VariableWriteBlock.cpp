#include <programGraph/VariableWriteBlock.h>
#include <utility/Literals.h>
#include <error/InternalError.h>

VariableWriteBlock::VariableWriteBlock(class GraphicalFunction& owningFunction, VariableIdentifier identifier)
	: StatementBlock(1, 1)
	, m_owningFunction(owningFunction)
	, m_identifier(std::move(identifier))
{
	if (!m_owningFunction.hasVariable(m_identifier))
	{
		THROW_ERROR(InternalError, "Created VariableWriteBlock with idenitifer: "_s + identifier + " which does not exist in owning function!");
	}
}

std::vector<Datatype> VariableWriteBlock::inputTypes() const
{
	return { m_owningFunction.variableType(m_identifier) };
}

std::vector<Datatype> VariableWriteBlock::outputTypes() const
{
	return { m_owningFunction.variableType(m_identifier) };
}

const VariableIdentifier& VariableWriteBlock::variableIdentifier() const
{
	return m_identifier;
}

