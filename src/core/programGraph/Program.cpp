#include <programGraph/Program.h>
#include <error/InternalError.h>

Program::Program(GraphicalFunction::UPtr startFunction, std::vector<GraphicalFunction::UPtr> functions)
{
	if (startFunction == nullptr)
	{
		THROW_ERROR(InternalError, "Program created with no starting function!");
	}
	m_startFunction = std::move(startFunction);
	m_functions = std::move(functions);
}

Program::Program(Program&& other)
{
	m_startFunction = std::move(other.m_startFunction);
	m_functions = std::move(other.m_functions);
}

std::vector<Value> Program::run(std::vector<Value> parameters)
{
	return { false };
}

