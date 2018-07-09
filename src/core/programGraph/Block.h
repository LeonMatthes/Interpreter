#pragma once
#include <vector>
#include <memory>
#include <programGraph/Datatype.h>
#include <programGraph/Connection.h>

class Block
{
public:
	using Ptr = std::shared_ptr<Block>;
	using WPtr = std::weak_ptr<Block>;
	
	virtual ~Block();

	virtual std::vector<Datatype> inputTypes() const = 0;
	virtual std::vector<Datatype> outputTypes() const = 0;
	
	std::vector<Connection> inputConnections() const;
	void setInputConnections(std::vector<Connection> val);

	MAKE_VISITABLE_VIRTUAL;
	
	size_t inputCount();
protected:
	Block() = delete;
	Block(size_t inputCount);
	
	std::vector<Connection> m_inputConnections;
private:
};