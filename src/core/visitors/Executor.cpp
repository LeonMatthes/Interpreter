#include <visitors/Executor.h>
#include <programGraph/ValueBlock.h>

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
	throw std::logic_error("The method or operation is not implemented.");
}

std::vector<Value> Executor::visit(class Connection& connection)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::vector<Value> Executor::visit(class PrimitiveFunction& PrimitiveFunction)
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::vector<Value> Executor::visit(class ValueBlock& valueBlock)
{
	return { valueBlock.value() };
}