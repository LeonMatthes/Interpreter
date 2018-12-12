#include <error/Error.h>
#include <error/InternalError.h>
#include <programGraph/ParameterAccessBlock.h>
#include <utility/Literals.h>

ParameterAccessBlock::ParameterAccessBlock(const GraphicalFunction& graphical)
	: ExpressionBlock(0)
	, m_function(graphical)
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

