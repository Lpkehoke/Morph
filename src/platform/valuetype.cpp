#include "platform/valuetype.h"

namespace platform
{

ValueType::ValueType(const ValueType& other)
  : m_name(other.m_name)
  , m_type_hash_code(other.m_type_hash_code)
{}

const std::string& ValueType::name() const
{
    return m_name;
}

bool ValueType::operator==(const ValueType& other) const
{
    return m_type_hash_code == other.m_type_hash_code;
}

} // namespace platform
