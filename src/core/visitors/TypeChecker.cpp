#include <visitors/TypeChecker.h>
#include <stdexcept>
#include <vector>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/FunctionBlock.h>

TypeChecker::TypeChecker()
{

}

TypeChecker::~TypeChecker()
{

}

bool TypeChecker::visit(class Function& function)
{
	//Functions don't have an implementation
	return false;
}


bool TypeChecker::checkOutputConnections(class GraphicalFunction &graphicalFunction)
{
	std::vector<FunctionBlock::Ptr> blocks = graphicalFunction.functionBlocks();
	if (blocks.size() == 0)
	{
		return false;
	}
	std::vector<Datatype> outputTypes = blocks.at(blocks.size() - 1)->outputTypes();
	if (outputTypes.size() != graphicalFunction.outputs().size())
	{
		return false;
	}

	for (size_t i = 0; i < outputTypes.size(); i++)
	{
		if (outputTypes.at(i) != graphicalFunction.outputs().at(i))
		{
			return false;
		}
	}
}

bool TypeChecker::visit(class GraphicalFunction& graphicalFunction)
{
	if (!checkOutputConnections(graphicalFunction))
	{
		return false;
	}

	for (FunctionBlock::Ptr block : graphicalFunction.functionBlocks())
	{
		if (!block->accept(*this))
		{
			return false;
		}
	}

	return true;
}

bool TypeChecker::visit(class FunctionBlock& functionBlock)
{
	for (size_t i = 0; i < functionBlock.inputConnections().size(); i++)
	{
		Connection connection = functionBlock.inputConnections().at(i);
		if (connection.isConnected() && connection.connectedType() != functionBlock.inputTypes().at(i))
		{
			return false;
		}
	}
	return true;
}

bool TypeChecker::visit(class Connection& connection)
{
	throw std::logic_error("The method or operation is not implemented.");
}
