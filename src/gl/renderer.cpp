#include "gl/renderer.h"

#include "gl/shader.h"

#include "rhi/drawcommandbuffer.h"

#include <glad/glad.h>

#include <fstream>
#include <streambuf>

namespace gl
{

void Renderer::render(const rhi::DrawCommandBuffer& draw_command_buffer)
{
    float vtx[] = {100.0f, -100.0f, 0.0f, -100.0f, -100.0f, 0.0f, 0.0f, 100.0f, 0.0f};

    GLuint vbo;
    GLuint vao;

    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(
        GL_ARRAY_BUFFER,
        9,
        vtx,
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    std::uint64_t next_element_idx = 0;

    for (const auto& draw_command : draw_command_buffer)
    {
        m_shader_collection[draw_command.shader_id]->set_current();

        glDrawArrays(GL_TRIANGLES, next_element_idx, draw_command.elements_num * 3);

        next_element_idx += draw_command.elements_num * 3;
    };
    
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

rhi::ShaderId Renderer::load_shader(const std::string& shader_name)
{
    // TODO: generalize fs operations in separate object.
    std::ifstream vertex_shader_stream;
    std::ifstream fragment_shader_stream;

    vertex_shader_stream.open("glsl/" + shader_name + ".glslv");
    fragment_shader_stream.open("glsl/" + shader_name + ".glslf");

    std::string vertex_shader_src(
        std::istreambuf_iterator<char>(vertex_shader_stream),
        std::istreambuf_iterator<char>{});

    std::string fragment_shader_src(
        std::istreambuf_iterator<char>(fragment_shader_stream),
        std::istreambuf_iterator<char>{});

    auto shader = std::make_shared<Shader>(vertex_shader_src, fragment_shader_src);
    auto shader_id = m_next_shader_id++;

    m_shader_collection.emplace(shader_id, std::move(shader));

    return shader_id;
}

} //namespace gl
