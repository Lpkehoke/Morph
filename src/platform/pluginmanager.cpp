#include "platform/pluginmanager.h"

#include "platform/knobschema.h"
#include "platform/nodefactory.h"

#include "base/registry.h"

#include <functional>


namespace platform
{

namespace
{

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

} // namespace 

struct PluginManager::Impl
{
    NodeFactoryRegistry node_factory_registry;
    KnobSchemaRegistry  knob_schema_registry;
};

PluginManager::PluginManager()
  : impl(new Impl)
{}

PluginManager::~PluginManager()
{
    delete impl;
}

void PluginManager::register_node_factory(NodeFactoryPtr factory)
{
    impl->node_factory_registry.register_entity(std::move(factory));
}

void PluginManager::register_knob_schema(KnobSchema knob_schema)
{
    impl->knob_schema_registry.register_entity(std::move(knob_schema));
}

const NodeFactoryPtr& PluginManager::get_node_factory(const std::string& node_model) const
{
    return impl->node_factory_registry.get(node_model);
}

const KnobSchema& PluginManager::get_knob_schema(const std::string& schema_name) const
{
    return impl->knob_schema_registry.get(schema_name);
}

} // namespace platform
