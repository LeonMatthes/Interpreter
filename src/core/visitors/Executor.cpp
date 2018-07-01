#include <visitors/Executor.h>
#include <programGraph/ValueBlock.h>
#include <programGraph/PrimitiveFunction.h>
#include <programGraph/FunctionBlock.h>

Executor::Executor()
{

}

std::vector<Value> Executor::visit(class Function& function)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::vector<Value> Executor::visit(class GraphicalFunction& graphicalFunction)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::vector<Value> Executor::visit(class FunctionBlock& functionBlock)
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

std::vector<Value> Executor::visit(class Connection& connection)
{
	std::vector<Value> outputs = connection.connectedBlock()->accept(*this);
	return { outputs.at(connection.connectedOutput()) };
}

std::vector<Value> Executor::visit(class PrimitiveFunction& primitiveFunction)
{
	std::vector<Value> results = primitiveFunction(m_callStack.top());
	m_callStack.pop();
	return results;
}

std::vector<Value> Executor::visit(class ValueBlock& valueBlock)
{
	return { valueBlock.value() };
}

void Executor::pushParameters(std::vector<Value> parameters)
{
	m_callStack.push(parameters);
}
