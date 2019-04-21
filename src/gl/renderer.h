#pragma once

#include "rhi/renderer.h"

#include <map>
#include <memory>

namespace gl { class Shader; }

namespace gl
{

class Renderer : public rhi::Renderer
{
  public:
    virtual void render(const rhi::DrawCommandBuffer& draw_command_buffer) override;
    virtual rhi::ShaderId load_shader(const std::string& shader_name) override;
  
  private:
    using ShaderPtr = std::shared_ptr<Shader>;
    using ShaderMap = std::map<rhi::ShaderId, ShaderPtr>;

    rhi::ShaderId   m_next_shader_id;
    ShaderMap       m_shader_collection;
};

} // namespace gl
