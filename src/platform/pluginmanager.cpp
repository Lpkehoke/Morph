#include "platform/pluginmanager.h"

namespace platform
{

void PluginManager::register_node_factory(NodeFactoryPtr factory)
{
    m_node_factory_registry.register_entity(std::move(factory));
}

void PluginManager::register_knob_schema(KnobSchema knob_schema)
{
    m_knob_schema_registry.register_entity(std::move(knob_schema));
}

const NodeFactoryPtr& PluginManager::get_node_factory(const std::string& node_model) const
{
    return m_node_factory_registry.get(node_model);
}

const KnobSchema& PluginManager::get_knob_schema(const std::string& schema_name) const
{
    return m_knob_schema_registry.get(schema_name);
}

} // namespace platform
