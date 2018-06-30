#include <gtest/gtest.h>
#include <programGraph/PrimitiveFunction.h>

TEST(TestPrimitiveFunction, add)
{
	EXPECT_EQ(Value(2.0), PrimitiveFunction::add({ 1.0, 1.0 }).at(0));
	EXPECT_EQ(Value(3.0), PrimitiveFunction::add({ 1.0, 2.0 }).at(0));
}

TEST(TestPrimitiveFunction, subtract)
{
	EXPECT_EQ(Value(0.0), PrimitiveFunction::subtract({ 1.0, 1.0 }).at(0));
	EXPECT_EQ(Value(1.0), PrimitiveFunction::subtract({ 2.0, 1.0 }).at(0));
}