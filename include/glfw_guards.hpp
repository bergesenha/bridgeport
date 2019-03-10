#pragma once

#include <utility>

#include "error.hpp"


class GLFWwindow;
class GLFWmonitor;

int glfwInit(void);
void glfwTerminate(void);
void glfwDestroyWindow(GLFWwindow*);
GLFWwindow* glfwCreateWindow(int, int, const char*, GLFWmonitor*, GLFWwindow*);


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


class window_context_guard
{
public:
    window_context_guard() = default;

    window_context_guard(int width,
                         int height,
                         std::string title,
                         GLFWmonitor* monitor = nullptr,
                         GLFWwindow* share = nullptr)
        : glfw_lib_(),
          window_(
              glfwCreateWindow(width, height, title.c_str(), monitor, share)),
          name_(title)
    {
    }

    window_context_guard(const window_context_guard&) = delete;

    window_context_guard(window_context_guard&& other)
        : glfw_lib_(std::move(other.glfw_lib_)),
          window_(other.window_),
          name_(std::move(other.name_))
    {
    }


    window_context_guard& operator=(const window_context_guard&) = delete;


    window_context_guard&
    operator=(window_context_guard&& other)
    {
        auto temp = std::move(other);

        swap(temp);

        return *this;
    }


    ~window_context_guard()
    {
        if(window_)
        {
            glfwDestroyWindow(window_);
        }
    }

    operator GLFWwindow*()
    {
        return window_;
    }

    void
    swap(window_context_guard& other)
    {
        std::swap(glfw_lib_, other.glfw_lib_);
        std::swap(window_, other.window_);
    }

private:
    glfw_init_guard glfw_lib_;
    GLFWwindow* window_;
    std::string name_;
};
} // namespace bp
