#include <programGraph/ParameterAccessBlock.h>
#include <error/InternalError.h>
#include <error/Error.h>
#include <utility/Literals.h>

ParameterAccessBlock::ParameterAccessBlock(GraphicalFunction& function)
	: ExpressionBlock(0)
	, m_function{function}
{
	if (m_function.inputs().empty())
	{
		THROW_ERROR(InternalError, "Tried to create ParameterAccessBlock for a function that doesn't have parameters");
	}
}

std::vector<Datatype> ParameterAccessBlock::inputTypes() const
{
	return {};
}

std::vector<Datatype> ParameterAccessBlock::outputTypes() const
{
	return m_function.inputs();
}

