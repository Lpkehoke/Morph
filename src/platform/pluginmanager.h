#pragma once

#include <memory>
#include <string>

namespace platform { class NodeFactory; }
namespace platform { class KnobSchema; }

namespace platform
{

using NodeFactoryPtr = std::shared_ptr<NodeFactory>;

class PluginManager
{
  public:
    PluginManager();
    ~PluginManager();

    void register_node_factory(NodeFactoryPtr factory);
    void register_knob_schema(KnobSchema knob_schema);

    const NodeFactoryPtr& get_node_factory(const std::string& node_model) const;
    const KnobSchema& get_knob_schema(const std::string& schema_name) const;

  private:
    struct Impl;
    Impl* impl;
};

} // namespace platform
