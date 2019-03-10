#pragma once

#include "error.hpp"


class GLFWwindow;

int glfwInit(void);
void glfwTerminate(void);


namespace bp
{
class glfw_init_guard
{
public:
    glfw_init_guard() : disabled_(false)
    {
        if(ref_count_ <= 0)
        {
            if(!glfwInit())
            {
                throw initialization_error(
                    "Unable to initialize glfw library.");
            }
        }

        ++ref_count_;
    }

    glfw_init_guard(const glfw_init_guard& other) : disabled_(other.disabled_)
    {
        if(!disabled_)
        {
            ++ref_count_;
        }
    }

    glfw_init_guard(glfw_init_guard&& other) : disabled_(other.disabled_)
    {
        // disable source so ref count is not incorrectly decremented.
        other.disabled_ = true;
    }

    glfw_init_guard&
    operator=(const glfw_init_guard& other)
    {
        ref_count_ +=
            static_cast<int>(disabled_) - static_cast<int>(other.disabled_);

        return *this;
    }

    glfw_init_guard&
    operator=(glfw_init_guard&& other)
    {
        if(!disabled_ && other.disabled_)
        {
            --ref_count_;
        }

        return *this;
    }


    ~glfw_init_guard()
    {
        if(!disabled_)
        {
            if(ref_count_ == 1)
            {
                glfwTerminate();
            }

            --ref_count_;
        }
    }

public:
    int
    user_count() const
    {
        return ref_count_;
    }

private:
    bool disabled_;

    static int ref_count_;
};
} // namespace bp
