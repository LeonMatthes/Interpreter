#include <programGraph/Connection.h>
#include <programGraph/Block.h>

bool Connection::isConnected()
{
	return false;
}

Block& Connection::connectedBlock()
{
	return *m_block;
}

size_t Connection::connectedOutput()
{
	return 0;
}

Datatype Connection::datatype()
{
	return Datatype::DOUBLE;
}

