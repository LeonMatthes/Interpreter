#pragma once
#include <vector>
#include <memory>
#include <programGraph/Datatype.h>
#include <programGraph/Connection.h>

class Block
{
public:
	typedef std::shared_ptr<Block> Ptr;
	typedef std::weak_ptr<Block> WPtr;
	
	virtual ~Block();

	virtual std::vector<Datatype> inputTypes() const = 0;
	virtual std::vector<Datatype> outputTypes() const = 0;
	
	std::vector<Connection> inputConnections() const;
	void setInputConnections(std::vector<Connection> val);
	
	size_t inputCount();
protected:
	Block() = delete;
	Block(size_t inputCount);
	
	std::vector<Connection> m_inputConnections;
private:
};