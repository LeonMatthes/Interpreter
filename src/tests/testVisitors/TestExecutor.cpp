#include <gtest/gtest.h>
#include <visitors/Executor.h>
#include <programGraph/Value.h>
#include <programGraph/ValueBlock.h>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/FunctionBlock.h>
#include <programGraph/PrimitiveFunction.h>
#include <error/Error.h>

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

TEST_F(TestExecutor, PrimitiveFunction)
{
	m_executor.pushParameters({ Value(2.0), Value(1.0) });
	EXPECT_EQ(Value(1.0), PrimitiveFunction::subtract.accept(m_executor).at(0));
}

TEST_F(TestExecutor, Connection)
{
	ValueBlock::Ptr block = std::make_shared<ValueBlock>(Value(false));
	Connection connection(block, 0);
	EXPECT_EQ(Value(false), connection.accept(m_executor).at(0));

	block = std::make_shared<ValueBlock>(Value(true));
	connection = Connection(block, 0);
	EXPECT_EQ(Value(true), connection.accept(m_executor).at(0));
}

TEST_F(TestExecutor, FunctionBlock)
{
	FunctionBlock block(PrimitiveFunction::subtract);
	//Unconnected block uses default for input types
	EXPECT_EQ(Value(0.0), block.accept(m_executor).at(0));
	
	ValueBlock::Ptr firstValue = std::make_shared<ValueBlock>(Value(2.0));
	ValueBlock::Ptr secondValue = std::make_shared<ValueBlock>(Value(1.0));
	block.setInputConnections({ Connection(firstValue, 0), Connection(secondValue, 0) });
	EXPECT_EQ(Value(1.0), block.accept(m_executor).at(0));

	block.setInputConnections({ Connection(firstValue, 0), Connection() });
	EXPECT_EQ(Value(2.0), block.accept(m_executor).at(0));
}


TEST_F(TestExecutor, GraphicalFunction)
{
	GraphicalFunction graphical({}, {Datatype::DOUBLE});
	FunctionBlock::Ptr functionBlock = std::make_shared<FunctionBlock>(PrimitiveFunction::subtract);
	
	ValueBlock::Ptr firstValue = std::make_shared<ValueBlock>(Value(2.0));
	ValueBlock::Ptr secondValue = std::make_shared<ValueBlock>(Value(1.0));
	functionBlock->setInputConnections({ Connection(firstValue, 0), Connection(secondValue, 0) });
	
	graphical.setFunctionBlocks({ firstValue, secondValue, functionBlock });
	EXPECT_EQ(Value(1.0), graphical.accept(m_executor).at(0));

	functionBlock = std::make_shared<FunctionBlock>(PrimitiveFunction::add);
	functionBlock->setInputConnections({ Connection(firstValue, 0), Connection(secondValue, 0) });

	graphical.setFunctionBlocks({ firstValue, secondValue, functionBlock });
	EXPECT_EQ(Value(3.0), graphical.accept(m_executor).at(0));
}

TEST_F(TestExecutor, Function)
{
	Function function;
	EXPECT_THROW(function.accept(m_executor), Error::Ptr);
}