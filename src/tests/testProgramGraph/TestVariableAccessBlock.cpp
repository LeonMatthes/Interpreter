#include <gtest/gtest.h>
#include <programGraph/VariableAccessBlock.h>
#include <programGraph/GraphicalFunction.h>
#include <testUtility/CustomAssert.h>
#include <error/InternalError.h>

class TestVariableAccessBlock : public ::testing::Test
{
public:
	TestVariableAccessBlock() = default;
	virtual ~TestVariableAccessBlock() = default;
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

TEST_F(TestVariableAccessBlock, TestOutputCount)
{
	ASSERT_EQ(1, VariableAccessBlock(graphical, identifier).outputCount());
	ASSERT_EQ(std::vector<Datatype>({ type }), VariableAccessBlock(graphical, identifier).outputTypes());
}

TEST_F(TestVariableAccessBlock, TestInvalidIdentifier)
{
	ASSERT_THROW_INTERPRETER(VariableAccessBlock(graphical, identifier + "invalidates identifier"), InternalError);
}