#include "node.h"

namespace platform
{

Node::Node(const Node& other)
    : m_input_knobs(other.m_input_knobs)
    , m_output_knobs(other.m_output_knobs)
{}

Node::Node(Node&& other)
    : m_input_knobs(std::move(other.m_input_knobs))
    , m_output_knobs(std::move(other.m_output_knobs))
{}

void Node::add_input_knob(KnobId&& knob_id, Knob&& knob)
{
    // TODO: implement in-place set
    m_input_knobs = m_input_knobs.set(knob_id, std::move(knob));
}

void Node::add_output_knob(KnobId&& knob_id, Knob&& knob)
{
    // TODO: implement in-place set
    m_output_knobs = m_output_knobs.set(knob_id, std::move(knob));
}

} // namespace platform
