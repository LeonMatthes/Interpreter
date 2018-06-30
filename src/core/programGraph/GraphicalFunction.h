#pragma once
#include <programGraph/Function.h>
#include <programGraph/ExpressionBlock.h>
#include <visitors/Visitor.h>
#include <vector>

class GraphicalFunction : public Function
{
public:
	GraphicalFunction(std::vector<Datatype> inputs, std::vector<Datatype> outputs);
	GraphicalFunction(const GraphicalFunction&) = delete;
	GraphicalFunction();
	virtual ~GraphicalFunction();
	
	std::vector<ExpressionBlock::Ptr> functionBlocks();
	void setFunctionBlocks(std::vector<ExpressionBlock::Ptr> blocks);

	MAKE_VISITABLE;

protected:
	std::vector<ExpressionBlock::Ptr> m_functionBlocks;
private:
};