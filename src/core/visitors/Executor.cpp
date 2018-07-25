#include <visitors/Executor.h>
#include <programGraph/ReturnBlock.h>
#include <error/InternalError.h>

Executor::Executor()
	: m_evaluator(*this)
{}

Executor::~Executor()
{

}

void Executor::visit(class GraphicalFunction& graphicalFunction)
{
	throw std::logic_error("The method or operation is not implemented.");
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

#include <programGraph/GraphicalFunction.h>
std::vector<Value> Executor::evaluate(class GraphicalFunction& graphicalFunction)
{
	if (!graphicalFunction.outputs().empty() && graphicalFunction.statementBlocks().empty())
	{
		THROW_ERROR(Error, "graphical function which should return a value is empty");
	}
	return {};
}

void Executor::throwExpressionError()
{
	THROW_ERROR(InternalError, "Executor cannot execute expressions!");
}

