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

TEST(TestValue, equality)
{
	EXPECT_FALSE(Value(Datatype::BOOLEAN) == Value(Datatype::DOUBLE));
	EXPECT_FALSE(Value(1.0) == Value(0.0));
	EXPECT_TRUE(Value(2.0) == Value(2.0));
	EXPECT_THROW(Value(Datatype::BOOLEAN) == Value(Datatype(-1)), Error::Ptr);
}

TEST(TestValue, initializeByType)
{
	EXPECT_EQ(Datatype::DOUBLE, Value(Datatype::DOUBLE).type());
	EXPECT_EQ(Datatype::BOOLEAN, Value(Datatype::BOOLEAN).type());
	EXPECT_EQ(0.0, Value(Datatype::DOUBLE).getDouble());
	EXPECT_EQ(false, Value(Datatype::BOOLEAN).getBoolean());
	EXPECT_THROW(Value(Datatype(-1)), Error::Ptr);
}