#pragma once

#include <cstdint>
#include <string>

namespace gl
{

class Shader
{
  public:
    Shader(
        const std::string& vertex_shader_src,
        const std::string& fragment_shader_src);
    
    ~Shader();

    void set_current() const;

  private:
    std::uint32_t m_vertex_shader_id;
    std::uint32_t m_fragment_shader_id;
    std::uint32_t m_program_id;
};

} // namespace gl
