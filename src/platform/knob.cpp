#include "knob.h"

#include <exception>
#include <memory>
#include <variant>


namespace platform
{

Knob::Knob(NodeId owner, KnobRef&& knob_ref)
    : m_owner(owner)
    , m_value(std::move(knob_ref))
{}

AttrId Knob::attribute(const std::string& attr_name) const
{
    try
    {
        return std::get<AttrMap>(m_value).get(attr_name);
    }
    catch (const std::bad_variant_access&)
    {
        throw std::runtime_error("Trying to get attribute of a knob reference.");
    }
}
    
NodeId Knob::owner() const
{
    return m_owner;
}

bool Knob::is_reference() const
{
    return std::get_if<KnobRef>(&m_value) != nullptr;
}

bool Knob::accept(const Knob& other) const
{
    try
    {
        const auto& attrs_map = std::get<AttrMap>(m_value);

        for (const auto& attr_pair : other)
        {
            const auto& attr_name = attr_pair.first;

            if (!attrs_map.contains(attr_name))
            {
                return false;
            }
        }
    }
    catch (const std::bad_variant_access&)
    {
        throw std::runtime_error("Reference knobs can't accept knobs.");
    }

    return true;
}

KnobPtr Knob::connect(const NodeId node_id, const std::string& knob_name)
{
    if (is_reference())
    {
        throw std::runtime_error("Trying to connect reference knob.");
    }

    KnobPtr result(new Knob(
        m_owner,
        KnobRef {node_id, knob_name}));

    return result;
}

Knob::AttrMap::Iterator Knob::begin() const
{
    try
    {
        return std::get<AttrMap>(m_value).begin();
    }
    catch (const std::bad_variant_access&)
    {
        throw std::runtime_error("Trying to iterate over attributes of a knob reference.");
    }
}

Knob::AttrMap::Iterator Knob::end() const
{
    try
    {
        return std::get<AttrMap>(m_value).end();
    }
    catch(const std::bad_variant_access&)
    {
        throw std::runtime_error("Trying to iterate over attributes of a knob reference.");
    }
}

} // namespace platform
