#include "gl/gl.h"

#include <glad/glad.h>

#include <stdexcept>

namespace gl
{

static bool opengl_initialized = false;

void init_opengl()
{
    if (!opengl_initialized && !gladLoadGL())
    {
        throw std::runtime_error("Failed to initialize OpenGL.");
    }
    else
    {
        opengl_initialized = true;
    }
}

} // namespace gl
