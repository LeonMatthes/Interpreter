#include <programGraph/ProgramFlowConnection.h>

#include <programGraph/StatementBlock.h>

ProgramFlowConnection::ProgramFlowConnection()
	: m_statement(std::shared_ptr<StatementBlock>(nullptr))
{}

ProgramFlowConnection::ProgramFlowConnection(std::shared_ptr<StatementBlock> statement)
	: m_statement(statement)
{}

ProgramFlowConnection::~ProgramFlowConnection()
{}

bool ProgramFlowConnection::isConnected()
{
	return !m_statement.expired();
}

