#include <programGraph/ProgramFlowConnection.h>

#include <programGraph/StatementBlock.h>
#include <error/InternalError.h>

ProgramFlowConnection::ProgramFlowConnection()
	: m_statement(std::shared_ptr<StatementBlock>(nullptr))
{}

ProgramFlowConnection::ProgramFlowConnection(std::shared_ptr<StatementBlock> statement)
	: m_statement(statement)
{}

ProgramFlowConnection::~ProgramFlowConnection()
{}

bool ProgramFlowConnection::isConnected() const
{
	return !m_statement.expired();
}

StatementBlock::Ptr ProgramFlowConnection::connectedStatement() const
{
	//lock before checking for isConnected(), because another thread may otherwise destruct the object
	//between the check and the lock, making the lock return nullptr, which we definitely don't want
	StatementBlock::Ptr statement = m_statement.lock();
	if (!isConnected())
	{
		THROW_ERROR(InternalError, "Called ProgramFlowConnection::connectedStatement, but it is not connected!");
	}
	return statement;
}

bool ProgramFlowConnection::operator==(const ProgramFlowConnection& other) const
{
	return m_statement.lock() == other.m_statement.lock();
}

