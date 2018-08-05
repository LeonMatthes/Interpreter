#include <gtest/gtest.h>
#include <programGraph/VariableReadBlock.h>
#include <programGraph/GraphicalFunction.h>
#include <testUtility/CustomAssert.h>
#include <error/InternalError.h>

class TestVariableReadBlock : public ::testing::Test
{
public:
	TestVariableReadBlock() = default;
	virtual ~TestVariableReadBlock() = default;
protected:

	virtual void SetUp() override
	{
		graphical.addVariable(identifier, type);
	}
	
	Datatype type = Datatype::DOUBLE;
	VariableIdentifier identifier = VariableIdentifier("My variable, for testing only!");
	GraphicalFunction graphical = GraphicalFunction({}, {});
private:
};

TEST_F(TestVariableReadBlock, TestOutput)
{
	ASSERT_EQ(1, VariableReadBlock(graphical, identifier).outputCount());
	ASSERT_EQ(std::vector<Datatype>({ type }), VariableReadBlock(graphical, identifier).outputTypes());
}

TEST_F(TestVariableReadBlock, TestInvalidIdentifier)
{
	ASSERT_THROW_INTERPRETER(VariableReadBlock(graphical, identifier + "invalidates identifier"), InternalError);
}