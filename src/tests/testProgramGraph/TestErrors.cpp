#include <gtest/gtest.h>
#include <error/Error.h>
#include <error/InternalError.h>

TEST(TestErrors, errorKeepsMessage)
{
	std::string errorMessage = "Hello World!";
	Error error(errorMessage);

	ASSERT_EQ(errorMessage, error.message());
}

TEST(TestErrors, internalErrorAppendsMessage)
{
	std::string errorMessage = "Hello World!";
	InternalError error(errorMessage);

	ASSERT_EQ(InternalError::prefix + errorMessage, error.message());
}

TEST(TestErrors, throwErrorMacro)
{
	std::string errorMessage("Hello World!");
	EXPECT_THROW(THROW_ERROR(Error, errorMessage), Error::Ptr);
	//Test for polymorphism
	EXPECT_THROW(THROW_ERROR(InternalError, errorMessage), Error::Ptr);

	try
	{
		THROW_ERROR(InternalError, errorMessage);
	}
	catch (std::shared_ptr<Error> error)
	{
		//Assert downcast still works
		ASSERT_NE(nullptr, std::dynamic_pointer_cast<InternalError>(error));
	}
	catch (...)
	{
		GTEST_FAIL();
	}
}