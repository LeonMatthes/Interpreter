#include <visitors/TypeChecker.h>
#include <error/InternalError.h>
#include <vector>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/FunctionBlock.h>

TypeChecker::TypeChecker()
{

}

TypeChecker::~TypeChecker()
{

}

bool TypeChecker::visit(class PrimitiveFunction& primitiveFunction)
{
	return false;
}

bool TypeChecker::visit(class ValueBlock& valueBlock)
{
	//ValueBlocks are always type safe as they don't depend on anything
	return true;
}

bool TypeChecker::checkOutputConnections(class GraphicalFunction &graphicalFunction)
{
	std::vector<ExpressionBlock::Ptr> blocks = graphicalFunction.expressionBlocks();
	if (blocks.empty())
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

	return true;
}

bool TypeChecker::visit(class GraphicalFunction& graphicalFunction)
{
	if (!checkOutputConnections(graphicalFunction))
	{
		return false;
	}

	for (const ExpressionBlock::Ptr& block : graphicalFunction.expressionBlocks())
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
	THROW_ERROR(InternalError, "Typechecker visited Connection");
}
