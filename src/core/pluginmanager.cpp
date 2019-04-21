#include "core/pluginmanager.h"

#include "core/dict.h"
#include "core/knobschema.h"
#include "core/nodefactory.h"
#include "core/value/defaultvalues.h"

#include "foundation/registry.h"

#include <utility>

using namespace foundation;

namespace core
{

namespace
{

class NodeFactoryRegistry : public Registry<NodeFactoryPtr>
{
  public:
    NodeFactoryRegistry()
      : Registry<NodeFactoryPtr>(
          [](const NodeFactoryPtr& entity)
          {
              return entity->node_info()["model"].cast<std::string>();
          }
      )
    {}
};

class KnobSchemaRegistry : public Registry<KnobSchema>
{
  public:
    KnobSchemaRegistry()
      : Registry<KnobSchema>(
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

Dict PluginManager::get_registered_nodes() const
{
    Dict result;
    
    for (const auto& pair : impl->node_factory_registry)
    {
        result[pair.first] = pair.second->node_info();
    }

    return result;
}

} // namespace core
