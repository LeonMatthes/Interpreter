#pragma once
#include <programGraph/Block.h>
#include <programGraph/ProgramFlowConnection.h>

class StatementBlock : public Block
{
public:
	virtual ~StatementBlock();
protected:
	StatementBlock(size_t inputCount, size_t flowConnectionsCount = 1);
	
private:
	std::vector<ProgramFlowConnection> m_flowConnections;
};