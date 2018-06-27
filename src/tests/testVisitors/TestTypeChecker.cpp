#include <gtest/gtest.h>
#include <visitors/TypeChecker.h>
#include <programGraph/Function.h>


class TestTypeChecker : public ::testing::Test
{
public:
	TypeChecker m_checker;
};

TEST_F(TestTypeChecker, FunctionIsFalse)
{
	Function function;
	ASSERT_FALSE(m_checker.visit(function));
}

#include <programGraph/GraphicalFunction.h>

TEST_F(TestTypeChecker, EmptyGraphicalFunction)
{
	GraphicalFunction grapical;
	ASSERT_TRUE(m_checker.visit(grapical));
}