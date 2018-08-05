#pragma once

#include <programGraph/StatementBlock.h>
#include <programGraph/GraphicalFunction.h>

class VariableWriteBlock : public StatementBlock
{
public:
	VariableWriteBlock(class GraphicalFunction& owningFunction, VariableIdentifier identifier);
	virtual ~VariableWriteBlock() = default;
protected:
	
	class GraphicalFunction& m_owningFunction;
	VariableIdentifier m_identifier;
private:
};