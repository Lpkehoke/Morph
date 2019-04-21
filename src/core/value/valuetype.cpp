#include "core/value/valuetype.h"

#include <utility>


namespace core
{

ValueType::ValueType(const ValueType& other)
  : m_name(other.m_name)
  , m_type_hash_code(other.m_type_hash_code)
{}

ValueType::ValueType(ValueType&& other)
{
    std::swap(m_name, other.m_name);
    std::swap(m_type_hash_code, other.m_type_hash_code);
}

const std::string& ValueType::name() const
{
    return m_name;
}

bool ValueType::operator==(const ValueType& other) const
{
    return m_type_hash_code == other.m_type_hash_code;
}

bool ValueType::operator!=(const ValueType& other) const
{
    return m_type_hash_code != other.m_type_hash_code;
}

} // namespace core
