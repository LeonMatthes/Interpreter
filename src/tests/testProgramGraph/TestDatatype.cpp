#include <gtest/gtest.h>
#include <programGraph/Datatype.h>
#include <error/Error.h>


TEST(TestDatatype, toString)
{
	EXPECT_EQ(std::string("double"), std::string("") + Datatype::DOUBLE);
	EXPECT_EQ(std::string("boolean"), std::string("") + Datatype::BOOLEAN);
	EXPECT_THROW(std::string("") + Datatype(-1), Error::Ptr);
}