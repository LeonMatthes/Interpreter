#pragma once
#include <programGraph/Function.h>
#include <programGraph/Block.h>
#include <vector>

class GraphicalFunction : public Function
{
public:
	GraphicalFunction();
	virtual ~GraphicalFunction() = default;
	
protected:
	std::vector<Block> m_blocks;
private:
};