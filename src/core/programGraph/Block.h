#pragma once
#include <memory>
#include <programGraph/Connection.h>
#include <programGraph/Datatype.h>
#include <vector>

class Block
{
public:
	using Ptr = std::shared_ptr<Block>;
	using WPtr = std::weak_ptr<Block>;
	using UPtr = std::unique_ptr<Block>;
	
	Block() = delete;
	virtual ~Block();

	virtual std::vector<Datatype> inputTypes() const = 0;
	virtual std::vector<Datatype> outputTypes() const = 0;
	
	std::vector<Connection> inputConnections() const;
	virtual void setInputConnections(std::vector<Connection> val);
	virtual void setInputConnection(size_t index, Connection connection);

	MAKE_VISITABLE_VIRTUAL;
	
	size_t inputCount();
	size_t outputCount();
protected:
	explicit Block(size_t inputCount);
	
	std::vector<Connection> m_inputConnections;
private:
};