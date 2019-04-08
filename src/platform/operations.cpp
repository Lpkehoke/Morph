#include "platform/operations.h"

#include "platform/attribute.h"
#include "platform/nodestoragetypes.h"
#include "platform/knob.h"
#include "platform/knobschema.h"
#include "platform/nodefactory.h"

#include <memory>
#include <utility>

namespace platform
{

NodeStorageState make_knob(NodeStorageState state, NodeId owner, const KnobSchema& knob_schema)
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

NodeStorageState make_node(NodeStorageState state, const NodeFactory& node_factory)
{
    auto next_state = std::move(state);

    const auto& knobs_info = node_factory.knobs_info();

    for (const auto& knob_schema : knobs_info.input_knobs_schema)
    {

    }

    for (const auto& knob_schema : knobs_info.output_knobs_schema)
    {

    }

    return next_state;
}

} // namespace platform
