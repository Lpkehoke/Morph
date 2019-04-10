#include "platform/platformcore.h"

#include "platform/knobschema.h"
#include "platform/logger.h"
#include "platform/nodestorage.h"
#include "platform/platformcoretypes.h"
#include "platform/pluginmanager.h"
#include "platform/valuetype.h"

#include <cstdint>
#include <memory>
#include <vector>

namespace platform
{

namespace
{

std::vector<ValueType> default_value_types()
{
    return {
        {"Float", ValueType::TypeTag<double>{}},
        {"Integer", ValueType::TypeTag<std::int64_t>{}}
    };
}

std::vector<KnobSchema> default_knob_shcemas(const PluginManagerPtr& plugin_manager)
{
    std::vector<KnobSchema> result;

    KnobSchema float_schema("FloatKnob");
    float_schema.add_attribute("Value", plugin_manager->make_value("Float", 0.0));
    result.push_back(std::move(float_schema));

    KnobSchema integer_schema("IntegerKnob");
    integer_schema.add_attribute("Value", plugin_manager->make_value("Integer", static_cast<std::int64_t>(0)));
    result.push_back(std::move(integer_schema));

    return result;
}

} // namespace


PlatformCore::PlatformCore()
{
    m_logger = std::make_shared<Logger>();
    m_plugin_manager = std::make_shared<PluginManager>();

    m_node_storage = std::make_shared<NodeStorage>(m_plugin_manager, m_logger);

    for (auto val_type : default_value_types())
    {
        m_plugin_manager->register_value_type(std::move(val_type));
    }

    for (auto knob_schema : default_knob_shcemas(m_plugin_manager))
    {
        m_plugin_manager->register_knob_schema(std::move(knob_schema));
    }
}

NodeStoragePtr PlatformCore::node_storage() const
{
    return m_node_storage;
}

PluginManagerPtr PlatformCore::plugin_manager() const
{
    return m_plugin_manager;
}

LoggerPtr PlatformCore::logger() const
{
    return m_logger;
}

} // namespace platform
