#include "platform/pluginmanager.h"

#include "platform/knobschema.h"
#include "platform/nodefactory.h"
#include "platform/valuetype.h"

#include "base/registry.h"

#include <functional>
#include <utility>

using namespace base;

namespace platform
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
              return entity->model();
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

class ValueTypeRegistry : public Registry<ValueType>
{
  public:
    ValueTypeRegistry()
      : Registry<ValueType>(
          [](const ValueType& entity)
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
    ValueTypeRegistry   value_type_registry;
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

void PluginManager::register_value_type(ValueType value_type)
{
    impl->value_type_registry.register_entity(std::move(value_type));
}

const NodeFactoryPtr& PluginManager::get_node_factory(const std::string& node_model) const
{
    return impl->node_factory_registry.get(node_model);
}

const KnobSchema& PluginManager::get_knob_schema(const std::string& schema_name) const
{
    return impl->knob_schema_registry.get(schema_name);
}

const ValueType& PluginManager::get_value_type(const std::string& value_type_name) const
{
    return impl->value_type_registry.get(value_type_name);
}

} // namespace platform
