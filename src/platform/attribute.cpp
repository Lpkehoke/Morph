#include "platform/attribute.h"

#include <utility>

namespace platform
{

Attribute::Attribute(Value value)
  : m_value(std::move(value))
{}

const Value& Attribute::get(float time) const
{
    return m_value;
}

} // namespace platform
