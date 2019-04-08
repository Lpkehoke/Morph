#include "value.h"

#include <utility>

namespace platform
{

Value::Value()
  : m_held(nullptr)
{}

Value::Value(const Value& other)
{
    if (!other.m_held)
    {
        m_held = nullptr;
    }

    m_held = other.m_held->clone();
}

Value::Value(Value&& other)
{
    std::swap(m_held, other.m_held);
}

Value::~Value()
{
    delete m_held;
}

const ValueType& Value::value_type() const
{
    if (!m_held)
    {
        throw std::runtime_error("Empty value has no type.");
    }

    return m_held->value_type();
}

} // namespace platform
