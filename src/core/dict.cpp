#include "core/dict.h"

#include "core/value/value.h"

#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace core
{

Value& Dict::operator[](const std::string& key)
{
    if (!m_data.contains(key))
    {
        m_data.mutable_set(key, Value());
    }

    return m_data.mutable_get(key);
}

const Value& Dict::operator[](const std::string& key) const
{
    return m_data.get(key);
}

void Dict::update(const Dict& other)
{
    for (const auto& pair : other.m_data)
    {
        m_data.mutable_set(pair.first, pair.second);
    }
}

Dict::Iterator Dict::begin() const
{
    return m_data.begin();
}

Dict::Iterator Dict::end() const
{
    return m_data.end();
}

bool Dict::contains(const std::string& key) const
{
    return m_data.contains(key);
}

bool Dict::operator==(const Dict& other) const
{
    return m_data == other.m_data;
}

} // namespace core
