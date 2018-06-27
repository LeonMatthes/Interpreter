#include <visitors/DAGChecker.h>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/FunctionBlock.h>
#include <algorithm>
#include <vector>

DAGChecker::DAGChecker()
{

}

DAGChecker::~DAGChecker()
{

}

bool DAGChecker::check(GraphicalFunction& functionGraph)
{
	std::vector<FunctionBlock::Ptr> functionBlocks = functionGraph.functionBlocks();
	if (functionBlocks.size() == 0)
	{
		return true;
	}
	bool result = depthFirstSearch(functionBlocks.at(functionBlocks.size() - 1));
	//cleanup to avoid holding pointers unnecessarily
	finished.clear(); 
	visited.clear();
	return result;
}

bool DAGChecker::depthFirstSearch(Block::Ptr block)
{
	if (std::find(finished.begin(), finished.end(), block) != finished.end())
	{
		return true;
	}

	if (std::find(visited.begin(), visited.end(), block) != visited.end()) 
	{
		return false; //Cyclic reference
	}

	visited.push_back(block);
	for (Connection& connection : block->inputConnections())
	{
		if (connection.isConnected() && !depthFirstSearch(connection.connectedBlock()))
		{
			return false;
		}
	}
	visited.remove(block);
	finished.push_back(block);
	return true;
}

