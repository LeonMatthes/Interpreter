#pragma once
#include <vector>
#include <programGraph/Datatype.h>
#include <programGraph/Connection.h>

class Function;

class Block
{
public:
	Block(Function& function);
	virtual ~Block() = default;

	std::vector<Datatype> inputTypes() const;
	std::vector<Datatype> outputTypes() const;

	Function& function() const;
protected:

	Function& m_function;
private:
};