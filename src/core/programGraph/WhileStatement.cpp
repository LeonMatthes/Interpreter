#include <programGraph/WhileStatement.h>

WhileStatement::WhileStatement()
	: StatementBlock(1, 2)
{

}

std::vector<Datatype> WhileStatement::inputTypes() const
{
	return { Datatype::BOOLEAN };
}

std::vector<Datatype> WhileStatement::outputTypes() const
{
	return {};
}