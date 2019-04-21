#pragma once

#include "core/value/value.h"

#include <vector>
#include <string>

namespace core
{

class KnobSchema
{
  public:
    struct AttributeInfo
    {
        std::string attr_name;
        Value       default_value;
    };

    using AttributeInfoCollection = std::vector<AttributeInfo>;
    using ConstIterator = AttributeInfoCollection::const_iterator;

    KnobSchema(std::string name);

    KnobSchema& add_attribute(std::string attr_name, Value default_value);

    const std::string name() const;

    ConstIterator begin() const;
    ConstIterator end() const;
  
  private:
    std::string             m_schema_name;
    AttributeInfoCollection m_attribute_info_collection;
};

} // namespace core
