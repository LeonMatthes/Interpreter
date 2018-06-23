#pragma once
#pragma once
#include <programGraph/Datatype.h>

class Block;

class Connection
{
public:
	Connection() = default;
	virtual ~Connection() = default;

	bool isConnected();
	Block& connectedBlock();
	size_t connectedOutput();
	Datatype datatype();

protected:
	Block* m_block;
	size_t m_output;
private:
};