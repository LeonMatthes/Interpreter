#include <programGraph/Function.h>

Function::Function(std::vector<Datatype> inputs, std::vector<Datatype> outputs)
	: m_inputs{ inputs }
	, m_outputs{ outputs }
{

}

std::vector<Datatype> Function::outputs() const
{
	return m_outputs;
}

std::vector<Datatype> Function::inputs() const
{
	return m_inputs;
}

