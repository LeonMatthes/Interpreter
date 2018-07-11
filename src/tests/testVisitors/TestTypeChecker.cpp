#include <gtest/gtest.h>
#include <visitors/TypeChecker.h>
#include <testProgramGraph/MockFunction.h>
#include <programGraph/Connection.h>
#include <programGraph/FunctionBlock.h>
#include <programGraph/ValueBlock.h>
#include <programGraph/PrimitiveFunction.h>

class TestTypeChecker : public ::testing::Test
{
public:
	TypeChecker m_checker;
};

TEST_F(TestTypeChecker, PrimitiveFunctionIsFalse)
{
	ASSERT_FALSE(PrimitiveFunction::add.accept(m_checker));
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
	
	MockFunction testFunction({}, {});
	FunctionBlock::Ptr block = std::make_shared<FunctionBlock>(testFunction);
	graphical.setExpressionBlocks({ block });
	ASSERT_FALSE(graphical.accept(m_checker));
	
	MockFunction testfunction2({}, { Datatype::BOOLEAN, Datatype::BOOLEAN });
	block = std::make_shared<FunctionBlock>(testfunction2);
	graphical.setExpressionBlocks({ block });
	ASSERT_FALSE(graphical.accept(m_checker));

	//the last block is the one being checked for matching Output
	MockFunction testFunction3({}, { Datatype::BOOLEAN, Datatype::DOUBLE });
	FunctionBlock::Ptr block2 = std::make_shared<FunctionBlock>(testFunction3);
	graphical.setExpressionBlocks({ block2, block });
	ASSERT_FALSE(graphical.accept(m_checker));
	graphical.setExpressionBlocks({ block, block2 });
	ASSERT_TRUE(graphical.accept(m_checker));
}

TEST_F(TestTypeChecker, GraphicalFunctionConnectionNotMatching)
{
	GraphicalFunction graphical({}, { Datatype::BOOLEAN, Datatype::DOUBLE });

	MockFunction testFunction({Datatype::BOOLEAN, Datatype::DOUBLE}, { Datatype::BOOLEAN, Datatype::DOUBLE });
	FunctionBlock::Ptr firstBlock = std::make_shared<FunctionBlock>(testFunction);
	FunctionBlock::Ptr secondBlock = std::make_shared<FunctionBlock>(testFunction);
	firstBlock->setInputConnections({ Connection(secondBlock, 1), Connection() });

	graphical.setExpressionBlocks({ secondBlock, firstBlock });
	ASSERT_FALSE(graphical.accept(m_checker));
}

TEST_F(TestTypeChecker, FunctionBlockWithoutConnections)
{
	MockFunction function;
	FunctionBlock block(function);
	ASSERT_TRUE(block.accept(m_checker));
}

TEST_F(TestTypeChecker, FunctionBlockChecksAllConnections)
{
	MockFunction testfunction({ Datatype::BOOLEAN, Datatype::DOUBLE }, { Datatype::DOUBLE});
	FunctionBlock::Ptr block = std::make_shared<FunctionBlock>(testfunction);
	block->setInputConnections({ Connection(block, 0), Connection() });
	ASSERT_FALSE(block->accept(m_checker));

	block->setInputConnections({ Connection(), Connection() });
	ASSERT_TRUE(block->accept(m_checker));

	block->setInputConnections({ Connection(), Connection(block, 0) });
	ASSERT_TRUE(block->accept(m_checker));
}

TEST_F(TestTypeChecker, ValueBlockNeedsNoTypeCheck)
{
	ValueBlock block(Value(false));
	EXPECT_EQ(true, block.accept(m_checker));
}