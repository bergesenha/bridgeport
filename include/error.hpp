#pragma once
#include <stdexcept>


namespace bp
{
class error : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class initialization_error : public error
{
public:
    using error::error;
};
} // namespace bp
