#pragma once
#include <utility>

#include <GL/glew.h>

namespace bp
{
class vao_bind_guard
{
public:
    vao_bind_guard() = delete;
    vao_bind_guard(const vao_bind_guard&) = delete;
    vao_bind_guard& operator=(const vao_bind_guard&) = delete;

    vao_bind_guard(GLuint vao) : vao_(vao), active_(true)
    {
        glBindVertexArray(vao_);
    }

    vao_bind_guard(vao_bind_guard&& other)
        : vao_(other.vao_), active_(other.active_)
    {
        other.active_ = false;
    }

    vao_bind_guard&
    operator=(vao_bind_guard&& other)
    {
        auto temp = std::move(other);
        std::swap(vao_, temp.vao_);
        std::swap(active_, temp.active_);

        return *this;
    }

    void
    release()
    {
        glBindVertexArray(0);
        active_ = false;
    }

    ~vao_bind_guard()
    {
        glBindVertexArray(0);
    }

private:
    GLuint vao_;
    bool active_;
};
} // namespace bp
