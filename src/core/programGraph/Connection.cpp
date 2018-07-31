#include <programGraph/Connection.h>
#include <programGraph/Block.h>
#include <error/InternalError.h>

Connection::Connection(std::shared_ptr<Block> block, size_t output)
	: m_block{ block }
	, m_output{ output }
{
	if (block->outputTypes().size() <= m_output)
	{
		THROW_ERROR(InternalError, std::string("Connection created with invalid output value:") + std::to_string(m_output));
	}
}

Connection::Connection()
	: m_block{}
{}

Connection::~Connection()
{}

bool Connection::isConnected() const
{
	return !m_block.expired();
}

std::shared_ptr<Block> Connection::connectedBlock() const
{
	//lock before checking for isConnected(), because another thread may otherwise destruct the object
	//between the check and the lock, making the lock return nullptr, which we definitely don't want
	Block::Ptr block = m_block.lock();
	if (!isConnected())
	{
		THROW_ERROR(InternalError, "Connection::connectedBlock called, but Connection is not connected")
	}
	return block;
}

size_t Connection::connectedOutput()
{
	if (!isConnected())
	{
		THROW_ERROR(InternalError, "Connection::connectedOutput called, but Connection is not connected");
	}
	return m_output;
}

Datatype Connection::connectedType() 
{
	return connectedBlock()->outputTypes().at(m_output);
}

