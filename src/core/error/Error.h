#pragma once
#include <memory>
#include <string>

#define THROW_ERROR(ErrorClass, ...) throw std::dynamic_pointer_cast<Error>(std::make_shared<ErrorClass>(__VA_ARGS__));

class Error {
public:
    using Ptr = std::shared_ptr<Error>;

    explicit Error(std::string message);

    virtual ~Error() = default;

    virtual std::string message() const;

protected:
    std::string m_message;

private:
};
