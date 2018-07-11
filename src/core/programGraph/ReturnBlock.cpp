#include <programGraph/ReturnBlock.h>
#include <programGraph/StatementBlock.h>
#include <programGraph/Function.h>

ReturnBlock::ReturnBlock(Function& function)
	: StatementBlock(function.outputs().size(), 0)
	, m_function(function)
{}

ReturnBlock::~ReturnBlock()
{

}

std::vector<Datatype> ReturnBlock::inputTypes() const
{
	return m_function.outputs();
}

std::vector<Datatype> ReturnBlock::outputTypes() const
{
	return {};
}

