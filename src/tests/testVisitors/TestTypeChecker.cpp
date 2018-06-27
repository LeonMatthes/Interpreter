#include <gtest/gtest.h>
#include <visitors/TypeChecker.h>
#include <programGraph/Function.h>
#include <programGraph/Connection.h>

class TestTypeChecker : public ::testing::Test
{
public:
	TypeChecker m_checker;
};

TEST_F(TestTypeChecker, FunctionIsFalse)
{
	Function function;
	ASSERT_FALSE(function.accept(m_checker));
}

TEST_F(TestTypeChecker, ConnectionThrowsError)
{
	Connection connection;
	ASSERT_ANY_THROW(connection.accept(m_checker));
}

#include <programGraph/GraphicalFunction.h>

TEST_F(TestTypeChecker, EmptyGraphicalFunction)
{
	GraphicalFunction graphical;
	ASSERT_FALSE(m_checker.visit(graphical));
}

TEST_F(TestTypeChecker, GraphicalFunctionOutput)
{
	GraphicalFunction graphical({}, {Datatype::BOOLEAN, Datatype::DOUBLE});
	
	Function testFunction({}, {});
	FunctionBlock::Ptr block = std::make_shared<FunctionBlock>(testFunction);
	graphical.setFunctionBlocks({ block });
	ASSERT_FALSE(graphical.accept(m_checker));
	
	Function testfunction2({}, { Datatype::BOOLEAN, Datatype::BOOLEAN });
	block = std::make_shared<FunctionBlock>(testfunction2);
	graphical.setFunctionBlocks({ block });
	ASSERT_FALSE(graphical.accept(m_checker));

	//the last block is the one being checked for matching Output
	Function testFunction3({}, { Datatype::BOOLEAN, Datatype::DOUBLE });
	FunctionBlock::Ptr block2 = std::make_shared<FunctionBlock>(testFunction3);
	graphical.setFunctionBlocks({ block2, block });
	ASSERT_FALSE(graphical.accept(m_checker));
	graphical.setFunctionBlocks({ block, block2 });
	ASSERT_TRUE(graphical.accept(m_checker));
}

TEST_F(TestTypeChecker, GraphicalFunctionConnectionNotMatching)
{
	GraphicalFunction graphical({}, { Datatype::BOOLEAN, Datatype::DOUBLE });

	Function testFunction({Datatype::BOOLEAN, Datatype::DOUBLE}, { Datatype::BOOLEAN, Datatype::DOUBLE });
	FunctionBlock::Ptr firstBlock = std::make_shared<FunctionBlock>(testFunction);
	FunctionBlock::Ptr secondBlock = std::make_shared<FunctionBlock>(testFunction);
	firstBlock->setInputConnections({ Connection(secondBlock, 1), Connection() });

	graphical.setFunctionBlocks({ secondBlock, firstBlock });
	ASSERT_FALSE(graphical.accept(m_checker));
}

TEST_F(TestTypeChecker, FunctionBlockWithoutConnections)
{
	Function function;
	FunctionBlock block(function);
	ASSERT_TRUE(block.accept(m_checker));
}

TEST_F(TestTypeChecker, FunctionBlockChecksAllConnections)
{
	Function testfunction({ Datatype::BOOLEAN, Datatype::DOUBLE }, { Datatype::DOUBLE});
	FunctionBlock::Ptr block = std::make_shared<FunctionBlock>(testfunction);
	block->setInputConnections({ Connection(block, 0), Connection() });
	ASSERT_FALSE(block->accept(m_checker));

	block->setInputConnections({ Connection(), Connection() });
	ASSERT_TRUE(block->accept(m_checker));

	block->setInputConnections({ Connection(), Connection(block, 0) });
	ASSERT_TRUE(block->accept(m_checker));
}