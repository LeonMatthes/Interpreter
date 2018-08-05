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
	ASSERT_TRUE(m_checker.visit(graphical)) << "Graphical functions can be empty, RuntimeError, not a TypeError";
}

#include <programGraph/ReturnBlock.h>
TEST_F(TestTypeChecker, GraphicalFunctionConnectionNotMatching)
{
	GraphicalFunction graphical({}, { Datatype::BOOLEAN, Datatype::DOUBLE });

	MockFunction testFunction({Datatype::BOOLEAN, Datatype::DOUBLE}, { Datatype::BOOLEAN, Datatype::DOUBLE });
	FunctionBlock::Ptr firstBlock = std::make_shared<FunctionBlock>(testFunction);
	auto returnBlock = std::make_shared<ReturnBlock>(testFunction);
	returnBlock->setInputConnections({ Connection(firstBlock, 0), Connection() });

	graphical.setExpressionBlocks({ firstBlock });
	graphical.setStatementBlocks({ returnBlock });
	ASSERT_TRUE(graphical.accept(m_checker));

	returnBlock->setInputConnections({ Connection(firstBlock, 1), Connection() });
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

TEST_F(TestTypeChecker, ReturnBlock)
{
	auto testfunction = MockFunction({ Datatype::BOOLEAN, Datatype::DOUBLE }, { Datatype::DOUBLE });
	auto returnBlock = ReturnBlock(testfunction);
	ASSERT_TRUE(returnBlock.accept(m_checker));

	auto valueBlock = std::make_shared<ValueBlock>(Value(Datatype::BOOLEAN));
	returnBlock.setInputConnections({ Connection(valueBlock, 0) });
	ASSERT_FALSE(returnBlock.accept(m_checker));
}

TEST_F(TestTypeChecker, SeperatedTypeMismatch)
{
	//TypeChecker should check every block, no matter if they are connected to anything important or not
	auto graphical = GraphicalFunction({}, {});

	auto valueBlock = std::make_shared<ValueBlock>(false);
	auto functionBlock = std::make_shared<FunctionBlock>(PrimitiveFunction::add);

	graphical.setExpressionBlocks({ valueBlock, functionBlock });
	ASSERT_TRUE(graphical.accept(m_checker));

	functionBlock->setInputConnections({ Connection(valueBlock, 0), Connection() });
	ASSERT_FALSE(graphical.accept(m_checker));
}