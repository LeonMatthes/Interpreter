#include <error/InternalError.h>
#include <gtest/gtest.h>
#include <programGraph/Datatype.h>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/VariableWriteBlock.h>
#include <testUtility/CustomAssert.h>

class TestVariableWriteBlock : public ::testing::Test {
public:
    TestVariableWriteBlock() = default;
    ~TestVariableWriteBlock() override = default;

protected:
    void SetUp() override
    {
        graphical.addVariable(identifier, type);
    }

    Datatype type = Datatype::DOUBLE;
    VariableIdentifier identifier = VariableIdentifier("My variable, for testing only!");
    GraphicalFunction graphical = GraphicalFunction({}, {});

private:
};

TEST_F(TestVariableWriteBlock, Creation)
{
    ASSERT_NO_THROW(VariableWriteBlock(graphical, identifier));
    ASSERT_THROW_INTERPRETER(VariableWriteBlock(graphical, identifier + "invalidate the identifier!"), InternalError);
}

TEST_F(TestVariableWriteBlock, Output)
{
    auto block = VariableWriteBlock(graphical, identifier);
    ASSERT_EQ(1, block.outputCount());
    ASSERT_EQ(std::vector<Datatype>({ type }), block.outputTypes());
}

TEST_F(TestVariableWriteBlock, Input)
{
    auto block = VariableWriteBlock(graphical, identifier);
    ASSERT_EQ(1, block.inputCount());
    ASSERT_EQ(std::vector<Datatype>({ type }), block.inputTypes());
}
