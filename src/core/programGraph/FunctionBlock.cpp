#include <programGraph/FunctionBlock.h>
#include <programGraph/Function.h>

FunctionBlock::FunctionBlock(Function& _function)
	: Block(_function.inputs().size())
	, m_function(_function)
{}

FunctionBlock::~FunctionBlock()
{}

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
