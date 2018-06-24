#pragma once
#include <programGraph/Function.h>
#include <programGraph/FunctionBlock.h>
#include <vector>

class GraphicalFunction : public Function
{
public:
	GraphicalFunction(const GraphicalFunction&) = delete;
	GraphicalFunction();
	virtual ~GraphicalFunction();
	
	const std::vector<FunctionBlock::UPtr>& functionBlocks();
	void setFunctionBlocks(std::vector<FunctionBlock::UPtr> blocks);
protected:
	std::vector<FunctionBlock::UPtr> m_functionBlocks;
private:
};