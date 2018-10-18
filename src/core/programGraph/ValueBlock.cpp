#include <programGraph/ValueBlock.h>


ValueBlock::ValueBlock(Value value)
	: ExpressionBlock(0)
	, m_value(value)
{}

ValueBlock::~ValueBlock()
{

}

std::vector<Datatype> ValueBlock::inputTypes() const
{
	return {};
}

std::vector<Datatype> ValueBlock::outputTypes() const
{
	return { value().type() };
}

Value ValueBlock::value() const
{
	return m_value;
}
