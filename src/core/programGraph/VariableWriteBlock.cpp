#include <programGraph/VariableWriteBlock.h>

VariableWriteBlock::VariableWriteBlock(class GraphicalFunction& owningFunction, VariableIdentifier identifier)
	: StatementBlock(1, 1)
	, m_owningFunction(owningFunction)
{

}

