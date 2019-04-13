#include "core/operations.h"

#include "core/attribute.h"
#include "core/knob.h"
#include "core/knobschema.h"
#include "core/node.h"
#include "core/nodefactory.h"
#include "core/nodestoragetypes.h"
#include "core/pluginmanager.h"

#include <map>
#include <memory>
#include <utility>
#include <variant>

namespace core
{

NodeStorageState make_knob(
    NodeStorageState    state,
    NodeId              owner,
    const KnobSchema&   knob_schema)
{
    auto next_state = std::move(state);

    Knob::AttrMap attr_map;

    for (const auto& attr_info : knob_schema)
    {
        auto attr_id = next_state.m_next_attr_id++;
        AttrPtr attribute = std::make_shared<Attribute>(attr_info.default_value);
        next_state.m_attributes.mutable_set(attr_id, attribute);

        attr_map.mutable_set(attr_info.attr_name, attr_id);
    }

    KnobPtr knob = std::make_shared<Knob>(owner, knob_schema.name(), std::move(attr_map));
    auto knob_id = next_state.m_next_knob_id++;

    next_state.m_knobs.mutable_set(knob_id, knob);
    
    return next_state;
}

NodeStorageState make_node(
    NodeStorageState        state,
    const PluginManager&    plugin_manager,
    const std::string&      node_model)
{
    auto next_state = std::move(state);

    const auto& node_factory = plugin_manager.get_node_factory(node_model);

    const auto node_id = next_state.m_next_node_id++;

    const Dict node_info = node_factory->node_info();
    Dict node_params;

    for (const auto& pair : node_info.get_as<Dict>("input_knobs_schema"))
    {
        const auto knob_id = next_state.m_next_knob_id;

        const auto& knob_schema_name = std::get<std::string>(pair.second);
        const auto& knob_schema = plugin_manager.get_knob_schema(knob_schema_name);

        next_state = make_knob(std::move(next_state), node_id, knob_schema);

        node_params.get_as<Dict>("input_knobs")[pair.first] = knob_id;
    }

    for (const auto& pair : node_info.get_as<Dict>("output_knobs_schema"))
    {
        const auto knob_id = next_state.m_next_knob_id;

        const auto& knob_schema_name = std::get<std::string>(pair.second);
        const auto& knob_schema = plugin_manager.get_knob_schema(knob_schema_name);
        next_state = make_knob(std::move(next_state), node_id, knob_schema);

        node_params.get_as<Dict>("output_knobs")[pair.first] = knob_id;
    }

    auto node = node_factory->create(std::move(node_params));
    next_state.m_nodes.mutable_set(node_id, std::move(node));

    return next_state;
}

} // namespace core
