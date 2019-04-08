#include "platform/knobschema.h"

#include <algorithm>
#include <stdexcept>
#include <utility>


namespace platform
{

KnobSchema::KnobSchema(std::string name)
  : m_schema_name(std::move(name))
{}

KnobSchema& KnobSchema::add_attribute(std::string attr_name, Value default_value)
{
    auto itr = std::find_if(
        m_attribute_info_collection.begin(),
        m_attribute_info_collection.end(),
        [=](const auto& elem)
        {
            return elem.attr_name == attr_name;
        });
    
    if (itr != m_attribute_info_collection.end())
    {
        throw std::runtime_error("Attribute already exists.");
    }

    m_attribute_info_collection.push_back({
        std::move(attr_name),
        std::move(default_value)});

    return *this;
}

const std::string KnobSchema::name() const
{
    return m_schema_name;
}

KnobSchema::ConstIterator KnobSchema::begin() const
{
    return m_attribute_info_collection.begin();
}

KnobSchema::ConstIterator KnobSchema::end() const
{
    return m_attribute_info_collection.end();
}

} // namespace platform
