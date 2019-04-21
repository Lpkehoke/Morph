#include "gl/shader.h"

#include <glad/glad.h>

#include <stdexcept>

namespace gl
{

Shader::Shader(
    const std::string& vertex_shader_src,
    const std::string& fragment_shader_src)
{
    m_vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    m_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    auto vertex_shader_src_ptr = vertex_shader_src.c_str();
    auto fragment_shader_src_ptr = fragment_shader_src.c_str();

    GLint compile_status = GL_FALSE;
    GLint link_status = GL_FALSE;

    glShaderSource(m_vertex_shader_id, 1, &vertex_shader_src_ptr, nullptr);
    glCompileShader(m_vertex_shader_id);
    glGetShaderiv(m_vertex_shader_id, GL_COMPILE_STATUS, &compile_status);

    if (!compile_status)
    {
        throw std::runtime_error("Error during vertex shader compilation.");
    }

    glShaderSource(m_fragment_shader_id, 1, &fragment_shader_src_ptr, nullptr);
    glCompileShader(m_fragment_shader_id);
    glGetShaderiv(m_fragment_shader_id, GL_COMPILE_STATUS, &compile_status);

    if (!compile_status)
    {
        throw std::runtime_error("Error during fragment shader compilation.");
    }

    m_program_id = glCreateProgram();
    glAttachShader(m_program_id, m_vertex_shader_id);
    glAttachShader(m_program_id, m_fragment_shader_id);
    glLinkProgram(m_program_id);
    glGetProgramiv(m_program_id, GL_LINK_STATUS, &link_status);

    if (!compile_status)
    {
        throw std::runtime_error("Error during shader linking.");
    }
}

Shader::~Shader()
{
    glDetachShader(m_program_id, m_vertex_shader_id);
    glDetachShader(m_program_id, m_fragment_shader_id);

    glDeleteShader(m_vertex_shader_id);
    glDeleteShader(m_fragment_shader_id);

    glDeleteProgram(m_program_id);

    // TODO: error checking.
}

void Shader::set_current() const
{
    glUseProgram(m_program_id);
}

} // namespace gl
