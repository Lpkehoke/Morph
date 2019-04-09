#pragma once

#include "platform/value.h"

#include <memory>
#include <string>

namespace platform { class NodeFactory; }
namespace platform { class KnobSchema; }
namespace platform { class ValueType; }

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
    void register_value_type(ValueType value_type);

    const NodeFactoryPtr& get_node_factory(const std::string& node_model) const;
    const KnobSchema& get_knob_schema(const std::string& schema_name) const;
    const ValueType& get_value_type(const std::string& value_type_name) const;

    template <typename T>
    Value make_value(const std::string& value_type_name, T value) const;

  private:
    struct Impl;
    Impl* impl;
};


//
//  PluginManager implementation.
//

template <typename T>
Value PluginManager::make_value(const std::string& value_type_name, T value) const
{
    const ValueType& value_type = get_value_type(value_type_name);
    return {std::move(value), value_type};
}

} // namespace platform
