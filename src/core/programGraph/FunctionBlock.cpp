#include <programGraph/FunctionBlock.h>
#include <programGraph/Function.h>

FunctionBlock::FunctionBlock(Function& function)
	: m_function{function}
	, Block(function.inputs().size())
{
}

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
