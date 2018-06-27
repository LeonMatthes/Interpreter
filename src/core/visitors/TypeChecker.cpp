#include <visitors/TypeChecker.h>
#include <stdexcept>

TypeChecker::TypeChecker()
{

}

TypeChecker::~TypeChecker()
{

}

bool TypeChecker::visit(class Function& function)
{
	return false;
}


bool TypeChecker::visit(class GraphicalFunction& graphicalFunction)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool TypeChecker::visit(class FunctionBlock& functionBlock)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool TypeChecker::visit(class Connection& connection)
{
	throw std::logic_error("The method or operation is not implemented.");
}
