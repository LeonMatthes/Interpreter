#include <programGraph/IfStatement.h>


IfStatement::IfStatement()
	: StatementBlock(1, 2)
{}

IfStatement::~IfStatement()
{}

std::vector<Datatype> IfStatement::inputTypes() const
{
	return { Datatype::BOOLEAN };
}

std::vector<Datatype> IfStatement::outputTypes() const
{
	return {};
}

