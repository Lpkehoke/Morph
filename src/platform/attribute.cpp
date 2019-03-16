#include "attribute.h"

namespace platform
{

Attribute::Attribute(const Attribute& other)
    : m_value(other.m_value)
{}

Attribute::Attribute(float value)
    : m_value(value)
{}

float Attribute::as_float() const
{
    return std::get<float>(m_value);
}

} // namespace platform
