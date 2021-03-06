#include "core/node.h"

#include "core/dict.h"
#include "core/knob.h"

#include <stdexcept>
#include <utility>
#include <variant>

namespace core { class Attribute; }

namespace core
{

Node::Node(const Dict& params)
    : m_state(nullptr)
{
    for (const auto& pair : params["input_knobs"].cast<Dict>())
    {
        m_input_knobs.mutable_set(pair.first, pair.second.cast<KnobId>());
    }

    for (const auto& pair : params["output_knobs"].cast<Dict>())
    {
        m_output_knobs.mutable_set(pair.first, pair.second.cast<KnobId>());
    }
}

void Node::compute() const
{}

Node::KnobMap Node::input_knobs() const
{
    return m_input_knobs;
}

Node::KnobMap Node::output_knobs() const
{
    return m_output_knobs;
}

void Node::on_compute_begin(NodeStorageState* state)
{
    m_state = state;
}

void Node::on_compute_end()
{
    m_state = nullptr;
}

const Attribute& Node::get_input_knob_attribute(
    const std::string& knob_name,
    const std::string& attr_name) const
{
    if (!m_state)
    {
        throw std::runtime_error("Compute has not begun.");
    }

    KnobId knob_id = m_input_knobs.get(knob_name);
    KnobPtr knob = m_state->m_knobs.get(knob_id);

    AttrId attr_id = knob->attribute(attr_name);
    AttrPtr attr = m_state->m_attributes.get(attr_id);

    return *attr;
}

Attribute& Node::get_output_knob_attribute(
    const std::string& knob_name,
    const std::string& attr_name) const
{
    if (!m_state)
    {
        throw std::runtime_error("Compute has not begun.");
    }

    KnobId knob_id = m_output_knobs.get(knob_name);
    KnobPtr knob = m_state->m_knobs.get(knob_id);

    AttrId attr_id = knob->attribute(attr_name);
    AttrPtr attr = m_state->m_attributes.mutable_get(attr_id);
    
    return *attr;
}

} // namespace core
