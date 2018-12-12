#include <programGraph/Function.h>
#include <programGraph/ReturnBlock.h>
#include <programGraph/StatementBlock.h>

ReturnBlock::ReturnBlock(Function& function)
	: StatementBlock(function.outputs().size(), 0)
	, m_function(function)
{}

ReturnBlock::~ReturnBlock()
= default;

std::vector<Datatype> ReturnBlock::inputTypes() const
{
	return m_function.outputs();
}

std::vector<Datatype> ReturnBlock::outputTypes() const
{
	return {};
}

