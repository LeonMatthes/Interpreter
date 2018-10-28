#include <gtest/gtest.h>
#include <programGraph/Datatype.h>
#include <error/InternalError.h>
#include <testUtility/CustomAssert.h>

TEST(TestDatatype, toString)
{
	EXPECT_EQ(std::string("Hello Double"), std::string("Hello ") + Datatype::DOUBLE);
	EXPECT_EQ(std::string("Boolean"), std::string("") + Datatype::BOOLEAN);
	ASSERT_THROW_INTERPRETER(std::string("") + Datatype(-1), InternalError);
}