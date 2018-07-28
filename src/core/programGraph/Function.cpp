#include <programGraph/Function.h>

Function::Function(std::vector<Datatype> inputs, std::vector<Datatype> outputs)
	: m_inputs( inputs )
	, m_outputs( outputs )
{}

Function::Function()
{}

Function::~Function()
{}

const std::vector<Datatype>& Function::outputs() const
{
	return m_outputs;
}

const std::vector<Datatype>& Function::inputs() const
{
	return m_inputs;
}

