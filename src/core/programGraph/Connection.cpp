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

bool Connection::isConnected()
{
	return !m_block.expired();
}

Block::Ptr Connection::connectedBlock()
{
	if (!isConnected())
	{
		THROW_ERROR(InternalError, "Connection::connectedBlock called, but Connection is not connected")
	}
	return m_block.lock();
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

