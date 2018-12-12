#include <programGraph/Function.h>

#include <utility>

#include <utility>

Function::Function(std::vector<Datatype> inputs, std::vector<Datatype> outputs)
	: m_inputs(std::move( std::move(inputs) ))
	, m_outputs(std::move( std::move(outputs) ))
{}

Function::Function()
= default;

Function::~Function()
= default;

const std::vector<Datatype>& Function::outputs() const
{
	return m_outputs;
}

const std::vector<Datatype>& Function::inputs() const
{
	return m_inputs;
}

