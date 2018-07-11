#pragma once
#include <list>
#include <programGraph/Block.h>

class GraphicalFunction;

class DAGChecker
{
public:
	DAGChecker();
	virtual ~DAGChecker();

	bool check(GraphicalFunction& functionGraph);


protected:
	bool depthFirstSearch(Block::Ptr block);
	
	std::list<Block::Ptr> finished;
	std::list<Block::Ptr> visited;
private:
};