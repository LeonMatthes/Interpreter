#include <programGraph/ProgramFlowConnection.h>

#include <error/InternalError.h>
#include <programGraph/StatementBlock.h>

ProgramFlowConnection::ProgramFlowConnection()
    : m_statement(std::shared_ptr<StatementBlock>(nullptr))
{
}

ProgramFlowConnection::ProgramFlowConnection(const std::shared_ptr<StatementBlock>& statement)
    : m_statement(statement)
{
}

ProgramFlowConnection::~ProgramFlowConnection()
    = default;

bool ProgramFlowConnection::isConnected() const
{
    return !m_statement.expired();
}

StatementBlock::Ptr ProgramFlowConnection::connectedStatement() const
{
    //lock before checking for isConnected(), because another thread may otherwise destruct the object
    //between the check and the lock, making the lock return nullptr, which we definitely don't want
    StatementBlock::Ptr statement = m_statement.lock();
    if (!isConnected()) {
        THROW_ERROR(InternalError, "Called ProgramFlowConnection::connectedStatement, but it is not connected!");
    }
    return statement;
}

bool ProgramFlowConnection::operator==(const ProgramFlowConnection& other) const
{
    return m_statement.lock() == other.m_statement.lock();
}
