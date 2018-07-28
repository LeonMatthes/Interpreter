#include <visitors/Executor.h>
#include <programGraph/ReturnBlock.h>
#include <error/InternalError.h>
#include <programGraph/GraphicalFunction.h>

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

#include <programGraph/ExpressionStatement.h>
void Executor::visit(class ExpressionStatement& expressionStatement)
{
	const auto& connectionToNext = expressionStatement.flowConnections().front();
	if (connectionToNext.isConnected())
	{
		connectionToNext.connectedStatement()->accept(*this);
	}
}


#include <error/RuntimeError.h>
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

void Executor::throwExpressionError()
{
	THROW_ERROR(InternalError, "Executor cannot execute expressions!");
}

