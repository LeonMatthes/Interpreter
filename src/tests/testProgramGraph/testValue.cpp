#include <gtest/gtest.h>
#include <programGraph/Value.h>
#include <error/Error.h>

TEST(TestValue, boolean)
{
	Value value(true);
	EXPECT_EQ(true, value.getBoolean());
	
	value = Value(false);
	EXPECT_EQ(false, value.getBoolean());
}

TEST(TestValue, double)
{
	Value value(1.0);
	EXPECT_EQ(1.0, value.getDouble());

	value = Value(0.0);
	EXPECT_EQ(0.0, value.getDouble());
}

TEST(TestValue, invalidGetType)
{
	Value value(true);
	EXPECT_THROW(value.getDouble(), Error::Ptr);

	value = Value(10.0);
	EXPECT_THROW(value.getBoolean(), Error::Ptr);
}