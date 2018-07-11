#include <gtest/gtest.h>

#include <programGraph/ReturnBlock.h>
#include <testProgramGraph/MockFunction.h>



TEST(TestReturnBlock, outputCount)
{
	MockFunction emptyFunction({}, {});
	ASSERT_EQ(0, ReturnBlock(emptyFunction).flowConnectionsCount());
}

TEST(TestReturnBlock, inputTypes)
{
	MockFunction emptyFunction({}, {});
	EXPECT_EQ(0, ReturnBlock(emptyFunction).inputCount());
	EXPECT_EQ(std::vector<Datatype>(), ReturnBlock(emptyFunction).inputTypes());

	MockFunction aFunction({}, { Datatype::DOUBLE });
	EXPECT_EQ(1, ReturnBlock(aFunction).inputCount());
	EXPECT_EQ(std::vector<Datatype>({ Datatype::DOUBLE }), ReturnBlock(aFunction).inputTypes());
}

TEST(TestReturnBlock, outputTypes)
{
	MockFunction aFunction({Datatype::BOOLEAN, Datatype::DOUBLE}, {Datatype::BOOLEAN});
	ReturnBlock block(aFunction);
	EXPECT_EQ(std::vector<Datatype>(), block.outputTypes());
}