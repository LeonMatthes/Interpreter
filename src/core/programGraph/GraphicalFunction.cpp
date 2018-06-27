#include <programGraph/GraphicalFunction.h>

GraphicalFunction::GraphicalFunction()
{}

GraphicalFunction::GraphicalFunction(std::vector<Datatype> inputs, std::vector<Datatype> outputs)
	: Function(inputs, outputs)
{}

GraphicalFunction::~GraphicalFunction()
{}

std::vector<FunctionBlock::Ptr> GraphicalFunction::functionBlocks()
{
	return m_functionBlocks;
}

void GraphicalFunction::setFunctionBlocks(std::vector<FunctionBlock::Ptr> blocks)
{
	m_functionBlocks = blocks;
}