#include "core/attribute.h"
#include "core/value.h"

#include <utility>

namespace core
{

Attribute::Attribute(Value value)
  : m_value(std::move(value))
{}

const Value& Attribute::get(float time) const
{
    return m_value;
}

} // namespace core
