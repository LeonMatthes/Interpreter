#include <programGraph/GraphicalFunction.h>

GraphicalFunction::GraphicalFunction()
{}

GraphicalFunction::GraphicalFunction(std::vector<Datatype> inputs, std::vector<Datatype> outputs)
	: Function(inputs, outputs)
{}

GraphicalFunction::~GraphicalFunction()
{}

std::vector<ExpressionBlock::Ptr> GraphicalFunction::functionBlocks()
{
	return m_functionBlocks;
}

void GraphicalFunction::setFunctionBlocks(std::vector<ExpressionBlock::Ptr> blocks)
{
	m_functionBlocks = blocks;
}