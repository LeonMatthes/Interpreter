#include <gtest/gtest.h>
#include <programGraph/PrimitiveFunction.h>

TEST(TestPrimitiveFunction, add)
{
	EXPECT_EQ(Value(2.0), PrimitiveFunction::add({ 1.0, 1.0 }).front());
	EXPECT_EQ(Value(3.0), PrimitiveFunction::add({ 1.0, 2.0 }).front());
}

TEST(TestPrimitiveFunction, subtract)
{
	EXPECT_EQ(Value(0.0), PrimitiveFunction::subtract({ 1.0, 1.0 }).front());
	EXPECT_EQ(Value(1.0), PrimitiveFunction::subtract({ 2.0, 1.0 }).front());
}

TEST(TestPrimitiveFunction, multiply)
{
	EXPECT_EQ(Value(1.0), PrimitiveFunction::multiply({ 1.0, 1.0 }).front());
	EXPECT_EQ(Value(6.0), PrimitiveFunction::multiply({ 2.0, 3.0 }).front());
}

TEST(TestPrimitiveFunction, divide)
{
	EXPECT_EQ(Value(1.0), PrimitiveFunction::divide({ 1.0, 1.0 }).front());
	EXPECT_EQ(Value(2.0), PrimitiveFunction::divide({ 2.0, 1.0 }).front());
	EXPECT_EQ(Value(3.0), PrimitiveFunction::divide({ 6.0, 2.0 }).front());
}