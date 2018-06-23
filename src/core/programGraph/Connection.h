#pragma once
#pragma once
#include <programGraph/Datatype.h>
#include <cstddef>

class Block;

class Connection
{
public:
	Connection();
	Connection(Block& block, size_t output);
	virtual ~Connection() = default;

	bool isConnected();
	Block& connectedBlock();
	size_t connectedOutput();
	Datatype connectedType();

protected:
	Block* m_block;
	size_t m_output;
private:
};