#include <visitors/TypeChecker.h>
#include <error/InternalError.h>
#include <vector>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/FunctionBlock.h>
#include <programGraph/ReturnBlock.h>
#include <programGraph/IfStatement.h>
#include <programGraph/WhileStatement.h>

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
	THROW_ERROR(InternalError, "TypeChecker visited PrimitiveFunction!");
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

bool TypeChecker::visit(class VariableReadBlock& variableReadBlock)
{
	return true;
}

bool TypeChecker::visit(class GraphicalFunction& graphicalFunction)
{
	const auto& expressionBlocks = graphicalFunction.expressionBlocks();
	for (auto& expression : expressionBlocks)
	{
		if (!expression->accept(*this))
		{
			return false;
		}
	}

	const auto& statementBlocks = graphicalFunction.statementBlocks();
	for (auto& statement : statementBlocks)
	{
		if (!statement->accept(*this))
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

#include <programGraph/VariableWriteBlock.h>
bool TypeChecker::visit(VariableWriteBlock& variableWriteBlock)
{
	return checkInputTypes(variableWriteBlock);
}

bool TypeChecker::visit(class IfStatement& ifStatement)
{
	return checkInputTypes(ifStatement);
}

bool TypeChecker::visit(class WhileStatement& whileStatement)
{
	return checkInputTypes(whileStatement);
}

bool TypeChecker::visit(class ParameterAccessBlock& parameterAccess)
{
	return true;
}
