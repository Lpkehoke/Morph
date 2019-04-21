#include "value.h"

#include <utility>

namespace core { class ValueType; }


namespace core
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
  : m_held(nullptr)
{
    std::swap(m_held, other.m_held);
}

Value& Value::operator=(const Value& other)
{
    delete m_held;
    m_held = other.m_held->clone();
    return *this;
}

Value& Value::operator=(Value&& other)
{
    delete m_held;
    m_held = std::exchange(other.m_held, nullptr);
    return *this;
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

bool Value::operator==(const Value& other) const
{
    if (!m_held)
    {
        return !other.m_held;
    }

    if (!other.m_held)
    {
        return false;
    }

    return *m_held == *other.m_held;
}

} // namespace core
