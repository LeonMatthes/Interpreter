#pragma once

#include <error/Error.h>
#include <gtest/gtest.h>
#include <memory>
#include <utility/Literals.h>

#define ASSERT_THROW_INTERPRETER(expression, error_type)                                                            \
    {                                                                                                               \
        bool thrown = false;                                                                                        \
        try {                                                                                                       \
            (expression);                                                                                           \
        } catch (Error::Ptr error_ptr) {                                                                            \
            ASSERT_NE(std::dynamic_pointer_cast<error_type>(error_ptr), std::shared_ptr<error_type>(nullptr))       \
                << "Expected Error Type: " << #error_type << "\nActually: It throws a different Interpreter-type!"; \
            thrown = true;                                                                                          \
        } catch (...) {                                                                                             \
            FAIL() << "Expected Error Type: " << #error_type << "\nActually: It throws a different type!";          \
        }                                                                                                           \
        if (!thrown)                                                                                                \
            FAIL() << "Expected Error Type: " << #error_type << "\nActually: It throws nothing!";                   \
    }
