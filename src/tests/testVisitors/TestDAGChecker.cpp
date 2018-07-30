#include <gtest/gtest.h>
#include <programGraph/FunctionBlock.h>
#include <testProgramGraph/MockFunction.h>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/ExpressionStatement.h>

#include <visitors/DAGChecker.h>

class TestDAGChecker : public ::testing::Test
{
public:
protected:
	GraphicalFunction m_graphical;
	DAGChecker m_checker;
private:
};

TEST_F(TestDAGChecker, emptyFunction)
{
	ASSERT_TRUE(m_checker.check(m_graphical));
}

TEST_F(TestDAGChecker, selfReference)
{
	MockFunction testFunction({ Datatype::BOOLEAN, Datatype::DOUBLE }, { Datatype::BOOLEAN, Datatype::DOUBLE });
	
	auto firstBlock = ExpressionStatement::make_shared<FunctionBlock>(testFunction);

	firstBlock->setInputConnections({ Connection(), Connection(firstBlock, 0) });
	m_graphical.setStatementBlocks({ firstBlock });
	ASSERT_FALSE(m_checker.check(m_graphical));

	firstBlock->setInputConnections({ Connection(firstBlock, 0), Connection() });
	ASSERT_FALSE(m_checker.check(m_graphical));
	
	auto secondExpression = std::unique_ptr<ExpressionBlock>(new FunctionBlock(testFunction));
	auto secondBlock = std::make_shared<ExpressionStatement>(std::move(secondExpression));
	secondBlock->setInputConnections({ Connection(secondBlock, 0), Connection() });
	firstBlock->setInputConnections({ Connection(), Connection() });
	m_graphical.setStatementBlocks({ firstBlock, secondBlock });
	ASSERT_FALSE(m_checker.check(m_graphical));
}

TEST_F(TestDAGChecker, cyclicReference)
{
	auto testFunction = MockFunction({ Datatype::BOOLEAN, Datatype::DOUBLE }, { Datatype::BOOLEAN, Datatype::DOUBLE });
	
	auto firstBlock = ExpressionStatement::make_shared<FunctionBlock>(testFunction);
	auto secondBlock = ExpressionStatement::make_shared<FunctionBlock>(testFunction);

	firstBlock->setInputConnections({ Connection(secondBlock, 0), Connection() });
	secondBlock->setInputConnections({ Connection(), Connection(firstBlock, 1) });
	m_graphical.setStatementBlocks({ secondBlock, firstBlock });
	ASSERT_FALSE(m_checker.check(m_graphical));

	//The checker should be able to run twice without problems
	firstBlock->setInputConnections({ Connection(), Connection() });
	ASSERT_TRUE(m_checker.check(m_graphical));
}

TEST_F(TestDAGChecker, unconnectedCyclicReference)
{
	MockFunction testFunction({ Datatype::BOOLEAN, Datatype::DOUBLE }, { Datatype::BOOLEAN, Datatype::DOUBLE });
	FunctionBlock::Ptr firstBlock = std::make_shared<FunctionBlock>(testFunction);
	FunctionBlock::Ptr secondBlock = std::make_shared<FunctionBlock>(testFunction);
	secondBlock->setInputConnections({ Connection(secondBlock, 1), Connection(secondBlock, 0) });

	//cyclic references not connected to main graph don't matter
	m_graphical.setExpressionBlocks({ secondBlock, firstBlock });
	ASSERT_TRUE(m_checker.check(m_graphical));
}

TEST_F(TestDAGChecker, DAG)
{
	auto testFunction = MockFunction({ Datatype::BOOLEAN, Datatype::DOUBLE }, { Datatype::BOOLEAN, Datatype::DOUBLE });

	auto firstBlock = ExpressionStatement::make_shared<FunctionBlock>(testFunction);
	auto secondBlock = ExpressionStatement::make_shared<FunctionBlock>(testFunction);
	auto thirdBlock = ExpressionStatement::make_shared<FunctionBlock>(testFunction);

	firstBlock->setInputConnections({ Connection(secondBlock, 0), Connection(thirdBlock, 0) });
	secondBlock->setInputConnections({ Connection(thirdBlock, 0), Connection(thirdBlock, 1) });

	m_graphical.setStatementBlocks({ thirdBlock, secondBlock, firstBlock });
	ASSERT_TRUE(m_checker.check(m_graphical));
}