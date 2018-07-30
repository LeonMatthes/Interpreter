#include <visitors/Evaluator.h>
#include <visitors/Executor.h>
#include <programGraph/ValueBlock.h>
#include <programGraph/PrimitiveFunction.h>
#include <programGraph/FunctionBlock.h>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/ExpressionStatement.h>
#include <error/InternalError.h>

Evaluator::Evaluator(class Executor& executor)
	: m_executor(executor)
{}

Evaluator::~Evaluator()
{}

std::vector<Value> Evaluator::visit(class GraphicalFunction& graphicalFunction)
{
	return m_executor.evaluate(graphicalFunction);
}

std::vector<Value> Evaluator::visit(class FunctionBlock& functionBlock)
{
	std::vector<Value> inputs;
	auto connections = functionBlock.inputConnections();

	for (size_t i = 0; i < connections.size(); i++)
	{
		Connection& connection = connections.at(i);
		if (connection.isConnected())
		{
			inputs.emplace_back(connection.accept(*this).front());
		}
		else
		{
			Value defaultValue(functionBlock.inputTypes().at(i));
			inputs.emplace_back(defaultValue);
		}
	}

	m_callStack.push(inputs);
	auto returnValues = functionBlock.function().accept(*this);
	m_callStack.pop();
	return returnValues;
}

std::vector<Value> Evaluator::visit(class Connection& connection)
{
	std::vector<Value> outputs = connection.connectedBlock()->accept(*this);
	return { outputs.at(connection.connectedOutput()) };
}

std::vector<Value> Evaluator::visit(class PrimitiveFunction& primitiveFunction)
{
	return primitiveFunction(m_callStack.top());
}

std::vector<Value> Evaluator::visit(class ValueBlock& valueBlock)
{
	return { valueBlock.value() };
}

std::vector<Value> Evaluator::visit(class ReturnBlock& ReturnBlock)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::vector<Value> Evaluator::visit(class ExpressionStatement& expressionStatement)
{
	return m_executor.evaluate(expressionStatement);
}

void Evaluator::pushParameters(std::vector<Value> parameters)
{
	m_callStack.push(parameters);
}
