#include <visitors/Executor.h>
#include <programGraph/ReturnBlock.h>

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
	throw std::logic_error("The method or operation is not implemented.");
}

void Executor::visit(class Connection& connection)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Executor::visit(class ValueBlock& valueBlock)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Executor::visit(class PrimitiveFunction& primitiveFunction)
{
	throw std::logic_error("The method or operation is not implemented.");
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

