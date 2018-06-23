#include <programGraph/Connection.h>
#include <programGraph/Block.h>
#include <error/InternalError.h>

Connection::Connection(Block& block, size_t output)
	: m_block{ &block }
	, m_output{ output }
{
	if (m_block->outputTypes().size() <= m_output)
	{
		THROW_ERROR(InternalError, std::string("Connection created with invalid output value:") + std::to_string(m_output));
	}
}

Connection::Connection()
	: m_block{ nullptr }
{}

bool Connection::isConnected()
{
	return m_block != nullptr;
}

Block& Connection::connectedBlock()
{
	if (!isConnected())
	{
		THROW_ERROR(InternalError, "Connection::connectedBlock called, but Connection is not connected")
	}
	return *m_block;
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
	return connectedBlock().outputTypes().at(m_output);
}

