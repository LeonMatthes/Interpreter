#pragma once
#include <programGraph/Function.h>
#include <programGraph/FunctionBlock.h>
#include <visitors/Visitor.h>
#include <vector>

class GraphicalFunction : public Function
{
public:
	GraphicalFunction(std::vector<Datatype> inputs, std::vector<Datatype> outputs);
	GraphicalFunction(const GraphicalFunction&) = delete;
	GraphicalFunction();
	virtual ~GraphicalFunction();
	
	std::vector<FunctionBlock::Ptr> functionBlocks();
	void setFunctionBlocks(std::vector<FunctionBlock::Ptr> blocks);

	MAKE_VISITABLE;

protected:
	std::vector<FunctionBlock::Ptr> m_functionBlocks;
private:
};