#include <algorithm>
#include <programGraph/FunctionBlock.h>
#include <programGraph/GraphicalFunction.h>
#include <vector>
#include <visitors/DAGChecker.h>

DAGChecker::DAGChecker()
= default;

DAGChecker::~DAGChecker()
= default;

bool DAGChecker::check(GraphicalFunction& functionGraph)
{
	const auto& blocks = functionGraph.statementBlocks();
	if (blocks.empty())
	{
		return true;
	}
	 
	for (const auto& block : blocks)
	{
		if (!depthFirstSearch(block))
		{
			cleanUp();
			return false;
		}
	}

	cleanUp();
	return true;
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

	visited.emplace_back(block);
	for (const auto& connection : block->inputConnections())
	{
		if (connection.isConnected() && !depthFirstSearch(connection.connectedBlock()))
		{
			return false;
		}
	}
	visited.remove(block);
	finished.emplace_back(block);
	return true;
}

void DAGChecker::cleanUp()
{
	//cleanup to avoid holding pointers unnecessarily
	finished.clear();
	visited.clear();
}

