#pragma once

#include "platform/knobschema.h"
#include "platform/nodefactory.h"

#include "base/registry.h"

#include <functional>
#include <memory>
#include <string>


namespace platform
{

using NodeFactoryPtr = std::shared_ptr<NodeFactory>;

class NodeFactoryRegistry : public base::Registry<NodeFactoryPtr>
{
  public:
    NodeFactoryRegistry()
      : base::Registry<NodeFactoryPtr>(
          [](const NodeFactoryPtr& factory)
          {
              return factory->model();
          }
      )
    {}
};

class KnobSchemaRegistry : public base::Registry<KnobSchema>
{
  public:
    KnobSchemaRegistry()
      : base::Registry<KnobSchema>(
          [](const KnobSchema& entity)
          {
              return entity.name();
          })
    {}
};

class PluginManager
{
  public:
    void register_node_factory(NodeFactoryPtr factory);
    void register_knob_schema(KnobSchema knob_schema);

    const NodeFactoryPtr& get_node_factory(const std::string& node_model) const;
    const KnobSchema& get_knob_schema(const std::string& schema_name) const;

  private:
    NodeFactoryRegistry m_node_factory_registry;
    KnobSchemaRegistry m_knob_schema_registry;
};

} // namespace platform
