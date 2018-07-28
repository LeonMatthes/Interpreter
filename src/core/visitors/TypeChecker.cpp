#include <visitors/TypeChecker.h>
#include <error/InternalError.h>
#include <vector>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/FunctionBlock.h>
#include <programGraph/ReturnBlock.h>

TypeChecker::TypeChecker()
{

}

TypeChecker::~TypeChecker()
{

}

bool TypeChecker::checkInputTypes(class Block& block)
{
	for (size_t i = 0; i < block.inputConnections().size(); i++)
	{
		Connection connection = block.inputConnections().at(i);
		if (connection.isConnected() && connection.connectedType() != block.inputTypes().at(i))
		{
			return false;
		}
	}
	return true;
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

bool TypeChecker::visit(class ReturnBlock& returnBlock)
{
	return checkInputTypes(returnBlock);
}

#include <programGraph/ExpressionStatement.h>
bool TypeChecker::visit(class ExpressionStatement& expressionStatement)
{
	return checkInputTypes(expressionStatement);
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
	return checkInputTypes(functionBlock);
}

bool TypeChecker::visit(class Connection& connection)
{
	THROW_ERROR(InternalError, "Typechecker visited Connection");
}
