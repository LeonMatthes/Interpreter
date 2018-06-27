#pragma once
#pragma once
#include <programGraph/Datatype.h>
#include <visitors/Visitor.h>
#include <cstddef>
#include <memory>

class Block;

class Connection
{
public:
	Connection();
	Connection(std::shared_ptr<Block> block, size_t output);
	virtual ~Connection();;

	bool isConnected();
	std::shared_ptr<Block> connectedBlock();
	size_t connectedOutput();
	Datatype connectedType();

	MAKE_VISITABLE;

protected:
	std::weak_ptr<Block> m_block;
	size_t m_output;
private:
};