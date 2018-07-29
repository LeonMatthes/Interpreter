#include <gtest/gtest.h>
#include <visitors/Executor.h>
#include <testProgramGraph/MockFunction.h>
#include <programGraph/ReturnBlock.h>
#include <programGraph/ValueBlock.h>
#include <error/InternalError.h>
#include <error/RuntimeError.h>
#include <testUtility/CustomAssert.h>


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
	ASSERT_THROW_INTERPRETER(functionBlock.accept(m_executor), InternalError);

	auto primitive = PrimitiveFunction::add;
	ASSERT_THROW_INTERPRETER(primitive.accept(m_executor), InternalError);

	auto connection = Connection();
	ASSERT_THROW_INTERPRETER(connection.accept(m_executor), InternalError);

	auto valueBlock = ValueBlock(false);
	ASSERT_THROW_INTERPRETER(valueBlock.accept(m_executor), InternalError);
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
	ASSERT_THROW_INTERPRETER(m_executor.evaluate(graphicalFunction), RuntimeError);
}

TEST_F(TestExecutor, SimpleGraphicalFunctionWithReturnTypes)
{
	auto returnType = Datatype::BOOLEAN;
	auto graphicalFunction = GraphicalFunction({}, { returnType });
	auto returnStatement = std::make_shared<ReturnBlock>(graphicalFunction);
	graphicalFunction.setStatementBlocks({ returnStatement });

	ASSERT_EQ(std::vector<Value>({ returnType }), m_executor.evaluate(graphicalFunction));

	auto value = Value(2.0);
	auto valueBlock = std::make_shared<ValueBlock>(value);
	graphicalFunction.setExpressionBlocks({ valueBlock });
	returnStatement->setInputConnections({ Connection(valueBlock, 0) });
	ASSERT_EQ(value, m_executor.evaluate(graphicalFunction).at(0));
}

#include <programGraph/ExpressionStatement.h>
TEST_F(TestExecutor, MultipleStatements)
{
	auto returnType = Datatype::DOUBLE;
	auto graphical = GraphicalFunction({}, { returnType });
	auto expressionBlock = std::unique_ptr<ExpressionBlock>(new ValueBlock(Value(0.0)));
	auto statement = std::make_shared<ExpressionStatement>(std::move(expressionBlock));
	graphical.setStatementBlocks({ statement });

	ASSERT_THROW_INTERPRETER(m_executor.evaluate(graphical), RuntimeError);

	auto returnBlock = std::make_shared<ReturnBlock>(graphical);
	statement->setFlowConnections({ProgramFlowConnection(returnBlock)});
	graphical.setStatementBlocks({ statement, returnBlock });
	ASSERT_EQ(std::vector<Value>({ returnType }), m_executor.evaluate(graphical));
}

TEST_F(TestExecutor, MultipleConnectedStatements)
{
	auto returnType = Datatype::DOUBLE;
	auto graphical = GraphicalFunction({}, { returnType });
	auto value = Value(20.0);
	auto expressionBlock = std::unique_ptr<ExpressionBlock>(new ValueBlock(value));
	auto expressionStatement = std::make_shared<ExpressionStatement>(std::move(expressionBlock));
	
	auto returnBlock = std::make_shared<ReturnBlock>(graphical);
	expressionStatement->setFlowConnections({ ProgramFlowConnection(returnBlock) });
	returnBlock->setInputConnections({ Connection(expressionStatement, 0) });
	graphical.setStatementBlocks({ expressionStatement, returnBlock });

	ASSERT_EQ(std::vector<Value>({ value }), m_executor.evaluate(graphical));
}

/*
TEST_F(TestExecutor, ForwardDataConnection)
{
	auto functionBlock = std::unique_ptr<ExpressionBlock>(new FunctionBlock(PrimitiveFunction::add));
	auto firstStatement = std::make_shared<ExpressionStatement>(std::move(functionBlock));
	
	auto valueBlock = std::unique_ptr<ExpressionBlock>(new ValueBlock(2.0));
	auto secondStatement = std::make_shared<ExpressionStatement>(std::move(valueBlock));
	
	firstStatement->setInputConnections({ Connection(secondStatement, 0), Connection()});
	auto graphical = GraphicalFunction();
	graphical.setStatementBlocks({ firstStatement, secondStatement });

	ASSERT_THROW_INTERPRETER(m_executor.evaluate(graphical), RuntimeError);
}*/