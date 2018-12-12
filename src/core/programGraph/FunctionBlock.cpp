#include <programGraph/Function.h>
#include <programGraph/FunctionBlock.h>

FunctionBlock::FunctionBlock(Function& _function)
	: ExpressionBlock(_function.inputs().size())
	, m_function(_function)
{}

FunctionBlock::~FunctionBlock()
= default;

std::vector<Datatype> FunctionBlock::inputTypes() const
{
	return function().inputs();
}

std::vector<Datatype> FunctionBlock::outputTypes() const
{
	return function().outputs();
}


Function& FunctionBlock::function() const
{
	return m_function;
}
