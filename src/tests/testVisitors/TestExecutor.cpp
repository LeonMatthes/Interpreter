#include <gtest/gtest.h>
#include <visitors/Executor.h>
#include <programGraph/Value.h>
#include <programGraph/ValueBlock.h>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/FunctionBlock.h>
#include <programGraph/PrimitiveFunction.h>

class TestExecutor : public ::testing::Test
{
public:
	Executor m_executor;
protected:
private:
};


TEST_F(TestExecutor, ValueBlock)
{
	Value value(false);
	std::vector<Value> values({ value });
	ValueBlock block(value);
	EXPECT_EQ(values, block.accept(m_executor));
}

/*
TEST_F(TestExecutor, GraphicalFunction)
{
	GraphicalFunction graphical;
	FunctionBlock::Ptr functionBlock = std::make_shared<FunctionBlock>(PrimitiveFunction::add);
	ValueBlock::Ptr firstValue = std::make_shared<ValueBlock>(Value(2.0));
	ValueBlock::Ptr secondValue = std::make_shared<ValueBlock>(Value(1.0));
	functionBlock->setInputConnections({ Connection(firstValue, 0), Connection(secondValue, 0) });
	graphical.setFunctionBlocks({ firstValue, secondValue, functionBlock });
	EXPECT_EQ(Value(1.0), graphical.accept(m_executor).at(0));
}*/