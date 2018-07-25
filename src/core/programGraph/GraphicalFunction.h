#pragma once
#include <programGraph/Function.h>
#include <programGraph/ExpressionBlock.h>
#include <programGraph/StatementBlock.h>
#include <visitors/Visitor.h>
#include <vector>

class GraphicalFunction : public Function
{
public:
	GraphicalFunction(std::vector<Datatype> inputs, std::vector<Datatype> outputs);
	GraphicalFunction(GraphicalFunction&& other); //Move needed for auto
	GraphicalFunction(const GraphicalFunction&) = delete;
	GraphicalFunction();
	virtual ~GraphicalFunction();
	
	std::vector<ExpressionBlock::Ptr> expressionBlocks();
	void setExpressionBlocks(std::vector<ExpressionBlock::Ptr> blocks);

	std::vector<StatementBlock::Ptr> statementBlocks();
	void setStatementBlocks(std::vector<StatementBlock::Ptr> blocks);

	MAKE_VISITABLE;

protected:
	std::vector<ExpressionBlock::Ptr> m_expressionBlocks;
	std::vector<StatementBlock::Ptr> m_statementBlocks;
private:
};