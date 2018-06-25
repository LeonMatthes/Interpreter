#include <gtest/gtest.h>
#include <programGraph/GraphicalFunction.h>

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
	Function testFunction({ Datatype::BOOLEAN, Datatype::DOUBLE }, { Datatype::BOOLEAN, Datatype::DOUBLE });
	
	
	FunctionBlock::Ptr firstBlock = std::make_shared<FunctionBlock>(testFunction);
	firstBlock->setInputConnections({ Connection(), Connection(firstBlock, 0) });

	m_graphical.setFunctionBlocks({ firstBlock });
	ASSERT_FALSE(m_checker.check(m_graphical));

	firstBlock->setInputConnections({ Connection(firstBlock, 0), Connection() });
	ASSERT_FALSE(m_checker.check(m_graphical));

	
	FunctionBlock::Ptr secondBlock = std::make_shared<FunctionBlock>(testFunction);
	secondBlock->setInputConnections({ Connection(secondBlock, 0), Connection() });
	firstBlock->setInputConnections({ Connection(secondBlock, 0), Connection() });
	m_graphical.setFunctionBlocks({ firstBlock, secondBlock });
	ASSERT_FALSE(m_checker.check(m_graphical));
}

TEST_F(TestDAGChecker, cyclicReference)
{
	Function testFunction({ Datatype::BOOLEAN, Datatype::DOUBLE }, { Datatype::BOOLEAN, Datatype::DOUBLE });
	FunctionBlock::Ptr firstBlock = std::make_shared<FunctionBlock>(testFunction);
	FunctionBlock::Ptr secondBlock = std::make_shared<FunctionBlock>(testFunction);

	firstBlock->setInputConnections({ Connection(secondBlock, 0), Connection() });
	secondBlock->setInputConnections({ Connection(), Connection(firstBlock, 1) });
	m_graphical.setFunctionBlocks({ secondBlock, firstBlock });
	ASSERT_FALSE(m_checker.check(m_graphical));

	//The checker should be able to run twice
	firstBlock->setInputConnections({ Connection(), Connection() });
	ASSERT_TRUE(m_checker.check(m_graphical));
}

TEST_F(TestDAGChecker, unconnectedCyclicReference)
{
	Function testFunction({ Datatype::BOOLEAN, Datatype::DOUBLE }, { Datatype::BOOLEAN, Datatype::DOUBLE });
	FunctionBlock::Ptr firstBlock = std::make_shared<FunctionBlock>(testFunction);
	FunctionBlock::Ptr secondBlock = std::make_shared<FunctionBlock>(testFunction);
	secondBlock->setInputConnections({ Connection(secondBlock, 1), Connection(secondBlock, 0) });

	//cyclic references not connected to last block don't matter
	m_graphical.setFunctionBlocks({ firstBlock, secondBlock });
	ASSERT_TRUE(m_checker.check(m_graphical));
}