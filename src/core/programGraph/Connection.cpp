#include <programGraph/Connection.h>
#include <programGraph/Block.h>

Connection::Connection(Block& block, size_t output)
	: m_block{ &block }
	, m_output{output}
{}

Connection::Connection()
	: m_block{ nullptr }
{

}

bool Connection::isConnected()
{
	return m_block != nullptr;
}

Block& Connection::connectedBlock()
{
	return *m_block;
}

size_t Connection::connectedOutput()
{
	return m_output;
}

Datatype Connection::datatype()
{
	return Datatype::DOUBLE;
}

