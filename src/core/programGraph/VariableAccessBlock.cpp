#include <programGraph/VariableAccessBlock.h>
#include <error/InternalError.h>
#include <utility/Literals.h>

VariableAccessBlock::VariableAccessBlock(const GraphicalFunction& owningFunction, VariableIdentifier identifier) : ExpressionBlock(0)
	, m_owningFunction(owningFunction)
	, m_identifier(std::move(identifier))
{
	if (!m_owningFunction.hasVariable(m_identifier))
	{
		THROW_ERROR(InternalError, "Created VariableAccessBlock with idenitifer: "_s + identifier + " which does not exist in owning function!");
	}
}

VariableAccessBlock::~VariableAccessBlock()
{}

std::vector<Datatype> VariableAccessBlock::inputTypes() const
{
	return {};
}

std::vector<Datatype> VariableAccessBlock::outputTypes() const
{
	return { m_owningFunction.variableType(m_identifier) };
}

