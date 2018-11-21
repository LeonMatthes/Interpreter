#include <visitors/TypeChecker.h>
#include <error/InternalError.h>
#include <vector>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/FunctionBlock.h>
#include <programGraph/ReturnBlock.h>
#include <programGraph/IfStatement.h>
#include <programGraph/WhileStatement.h>

TypeChecker::TypeChecker()
	: m_currentFunction(nullptr)
{

}

TypeChecker::~TypeChecker()
{

}

TypeChecker::VisitorType TypeChecker::checkInputTypes(class Block& block)
{
	for (size_t i = 0; i < block.inputConnections().size(); i++)
	{
		Connection connection = block.inputConnections().at(i);
		if (connection.isConnected() && connection.connectedType() != block.inputTypes().at(i))
		{
			return TypeCheckResult(m_currentFunction, &block);
		}
	}
	return true;
}

TypeChecker::VisitorType TypeChecker::visit(class PrimitiveFunction& primitiveFunction)
{
	THROW_ERROR(InternalError, "TypeChecker visited PrimitiveFunction!");
}

TypeChecker::VisitorType TypeChecker::visit(class ValueBlock& valueBlock)
{
	//ValueBlocks are always type safe as they don't depend on anything
	return true;
}

TypeChecker::VisitorType TypeChecker::visit(class ReturnBlock& returnBlock)
{
	return checkInputTypes(returnBlock);
}

#include <programGraph/ExpressionStatement.h>
TypeChecker::VisitorType TypeChecker::visit(class ExpressionStatement& expressionStatement)
{
	return checkInputTypes(expressionStatement);
}

TypeChecker::VisitorType TypeChecker::visit(class VariableReadBlock& variableReadBlock)
{
	return true;
}

TypeChecker::VisitorType TypeChecker::visit(class GraphicalFunction& graphicalFunction)
{
	m_currentFunction = &graphicalFunction;

	auto result = TypeCheckResult();
	const auto& expressionBlocks = graphicalFunction.expressionBlocks();
	for (auto& expression : expressionBlocks)
	{
		result.merge(expression->accept(*this));
	}

	const auto& statementBlocks = graphicalFunction.statementBlocks();
	for (auto& statement : statementBlocks)
	{
		result.merge(statement->accept(*this));
	}
	return result;
}

TypeChecker::VisitorType TypeChecker::visit(class FunctionBlock& functionBlock)
{
	return checkInputTypes(functionBlock);
}

TypeChecker::VisitorType TypeChecker::visit(class Connection& connection)
{
	THROW_ERROR(InternalError, "Typechecker visited Connection");
}

#include <programGraph/VariableWriteBlock.h>
TypeChecker::VisitorType TypeChecker::visit(VariableWriteBlock& variableWriteBlock)
{
	return checkInputTypes(variableWriteBlock);
}

TypeChecker::VisitorType TypeChecker::visit(class IfStatement& ifStatement)
{
	return checkInputTypes(ifStatement);
}

TypeChecker::VisitorType TypeChecker::visit(class WhileStatement& whileStatement)
{
	return checkInputTypes(whileStatement);
}

TypeChecker::VisitorType TypeChecker::visit(class ParameterAccessBlock& parameterAccess)
{
	return true;
}
