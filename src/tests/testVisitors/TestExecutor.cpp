#include <gtest/gtest.h>
#include <visitors/Executor.h>
#include <testProgramGraph/MockFunction.h>
#include <programGraph/ReturnBlock.h>
#include <programGraph/ValueBlock.h>


class TestExecutor : public ::testing::Test
{
protected:
	Executor m_executor;
};

TEST_F(TestExecutor, EmptyReturnBlock)
{
	MockFunction emptyFunction({}, {});
	ReturnBlock block(emptyFunction);
	ASSERT_THROW(block.accept(m_executor), Return);
	try
	{
		block.accept(m_executor);
	}
	catch (Return& returnValues)
	{
		EXPECT_TRUE(returnValues.m_values.empty());
	}
}

TEST_F(TestExecutor, UnconnectedReturnBlock)
{
	MockFunction myFunction({}, { Datatype::DOUBLE });
	ReturnBlock block(myFunction);
	ASSERT_THROW(block.accept(m_executor), Return);
	try
	{
		block.accept(m_executor);
	}
	catch (Return& returnValues)
	{
		EXPECT_EQ(std::vector<Value>({ Value(Datatype::DOUBLE) }), returnValues.m_values);
	}
}

TEST_F(TestExecutor, ConnectedReturnBlock)
{
	Value returnedValue(2.0);

	MockFunction myFunction({}, { Datatype::DOUBLE });
	ValueBlock::Ptr valueBlock = std::make_shared<ValueBlock>(returnedValue);
	ReturnBlock returnBlock(myFunction);
	returnBlock.setInputConnections({ Connection(valueBlock, 0) });

	ASSERT_THROW(returnBlock.accept(m_executor), Return);
	try
	{
		returnBlock.accept(m_executor);
	}
	catch (Return& returnValues)
	{
		ASSERT_EQ(std::vector<Value>({ returnedValue }), returnValues.m_values);
	}
}

#include <programGraph/FunctionBlock.h>
#include <programGraph/PrimitiveFunction.h>
#include <programGraph/ValueBlock.h>
#include <error/Error.h>
TEST_F(TestExecutor, ExpressionBlocksNotCorrect)
{
	auto emptyFunction = MockFunction({}, {});
	auto functionBlock = FunctionBlock(emptyFunction);
	ASSERT_THROW(functionBlock.accept(m_executor), Error::Ptr);

	auto primitive = PrimitiveFunction::add;
	ASSERT_THROW(primitive.accept(m_executor), Error::Ptr);

	auto connection = Connection();
	ASSERT_THROW(connection.accept(m_executor), Error::Ptr);

	auto valueBlock = ValueBlock(false);
	ASSERT_THROW(valueBlock.accept(m_executor), Error::Ptr);
}

#include <programGraph/GraphicalFunction.h>
TEST_F(TestExecutor, EmptyGraphicalFunction)
{
	//Empty Graphical Function, does NOT! have a return block
	GraphicalFunction graphicalFunction;
	ASSERT_EQ(std::vector<Value>(), m_executor.evaluate(graphicalFunction));
}

TEST_F(TestExecutor, EmptyGraphicalFunctionWithReturnTypes)
{
	auto graphicalFunction = GraphicalFunction({}, { Datatype::BOOLEAN });
	ASSERT_THROW(m_executor.evaluate(graphicalFunction), Error::Ptr);
}

TEST_F(TestExecutor, SimpleGraphicalFunctionWithReturnTypes)
{
	auto returnType = Datatype::BOOLEAN;
	auto graphicalFunction = GraphicalFunction({}, { returnType });
	graphicalFunction.setStatementBlocks({ std::make_shared<ReturnBlock>(graphicalFunction) });

	//ASSERT_EQ(std::vector<Value>({ returnType }), m_executor.evaluate(graphicalFunction));
}