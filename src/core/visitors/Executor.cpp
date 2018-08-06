#include <visitors/Executor.h>
#include <programGraph/ReturnBlock.h>
#include <error/InternalError.h>
#include <programGraph/GraphicalFunction.h>

#include <programGraph/ExpressionStatement.h>
#include <error/RuntimeError.h>
Executor::Executor()
	: m_evaluator(*this)
{}

Executor::~Executor()
{

}

void Executor::visit(class GraphicalFunction& graphicalFunction)
{
	if (!graphicalFunction.statementBlocks().empty())
	{
		graphicalFunction.statementBlocks().front()->accept(*this);
	}
}

void Executor::visit(class FunctionBlock& functionBlock)
{
	throwExpressionError();
}

void Executor::visit(class Connection& connection)
{
	throwExpressionError();
}

void Executor::visit(class ValueBlock& valueBlock)
{
	throwExpressionError();
}

void Executor::visit(class PrimitiveFunction& primitiveFunction)
{
	throwExpressionError();
}

void Executor::visit(class ReturnBlock& returnBlock)
{
	Return returnValues;
	std::vector<Connection> connections = returnBlock.inputConnections();
	for (size_t i = 0; i < connections.size(); i++)
	{
		Connection connection = connections.at(i);
		if (connection.isConnected())
		{
			returnValues.m_values.emplace_back(connection.accept(m_evaluator).at(0));
		}
		else
		{
			returnValues.m_values.emplace_back(Value(returnBlock.inputTypes().at(i)));
		}
	}

	throw returnValues;
}

void Executor::visit(class ExpressionStatement& expressionStatement)
{
	m_executedStatements[&expressionStatement] = expressionStatement.expression().accept(m_evaluator);
	const auto& connectionToNext = expressionStatement.flowConnections().front();
	if (connectionToNext.isConnected())
	{
		connectionToNext.connectedStatement()->accept(*this);
	}
}


void Executor::visit(class VariableReadBlock& variableReadBlock)
{
	throwExpressionError();
}

void Executor::visit(class VariableWriteBlock& variableWriteBlock)
{
	
}

std::vector<Value> Executor::evaluate(class GraphicalFunction& graphicalFunction)
{
	try
	{
		graphicalFunction.accept(*this);
	}
	catch (const Return& returnValues)
	{
		return returnValues.m_values;
	}

	if (!graphicalFunction.outputs().empty())
	{
		THROW_ERROR(RuntimeError, "graphical function which should return a value, did not reach a return statement!");
	}

	return {};
}

std::vector<Value> Executor::evaluate(class StatementBlock& statement)
{
	auto savedState = m_executedStatements.find(&statement);
	if (savedState == m_executedStatements.end())
	{
		THROW_ERROR(RuntimeError, "Forward data connection detected!");
	}
	return savedState->second;
}

void Executor::throwExpressionError()
{
	THROW_ERROR(InternalError, "Executor cannot execute expressions!");
}

