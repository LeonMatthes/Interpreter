#include <visitors/Evaluator.h>
#include <programGraph/ValueBlock.h>
#include <programGraph/PrimitiveFunction.h>
#include <programGraph/FunctionBlock.h>
#include <programGraph/GraphicalFunction.h>
#include <error/InternalError.h>

Evaluator::Evaluator()
{}

Evaluator::~Evaluator()
{}

std::vector<Value> Evaluator::visit(class GraphicalFunction& graphicalFunction)
{
	std::vector<ExpressionBlock::Ptr> blocks = graphicalFunction.expressionBlocks();
	std::vector<Value> returnValues = blocks.at(blocks.size() - 1)->accept(*this);
	m_callStack.pop();
	return returnValues;
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
			inputs.push_back(connection.accept(*this).at(0));
		}
		else
		{
			Value defaultValue(functionBlock.inputTypes().at(i));
			inputs.push_back(defaultValue);
		}
	}

	m_callStack.push(inputs);

	return functionBlock.function().accept(*this);
}

std::vector<Value> Evaluator::visit(class Connection& connection)
{
	std::vector<Value> outputs = connection.connectedBlock()->accept(*this);
	return { outputs.at(connection.connectedOutput()) };
}

std::vector<Value> Evaluator::visit(class PrimitiveFunction& primitiveFunction)
{
	std::vector<Value> results = primitiveFunction(m_callStack.top());
	m_callStack.pop();
	return results;
}

std::vector<Value> Evaluator::visit(class ValueBlock& valueBlock)
{
	return { valueBlock.value() };
}

void Evaluator::pushParameters(std::vector<Value> parameters)
{
	m_callStack.push(parameters);
}
