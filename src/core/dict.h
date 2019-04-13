#pragma once

#include "foundation/immutable/map.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <variant>

namespace core
{

class Dict
{
  public:
    using Value = std::variant<std::monostate, Dict, std::string, double, bool, std::uint64_t>;
    using Iterator = foundation::Map<std::string, Value>::Iterator;

    Value& operator[](const std::string& key);
    const Value& operator[](const std::string& key) const;

    template <typename T>
    const T& get_as(const std::string& key) const;

    template <typename T>
    T& get_as(const std::string& key);

    void update(const Dict& other);

    Iterator begin() const;
    Iterator end() const;

    bool contains(const std::string& key) const;
    bool operator==(const Dict& other) const;

    std::string to_string() const;

  private:
    foundation::Map<std::string, Value> m_data;
};

std::ostream& operator<<(std::ostream &os, const Dict &dict);

//
//  Dict implementation.
//

template <typename T>
const T& Dict::get_as(const std::string& key) const
{
    const Value& value = m_data.get(key);

    try
    {
        return std::get<T>(value);
    }
    catch (const std::bad_variant_access&)
    {
        throw std::runtime_error("Dict value type mismatch.");
    }
}

template <typename T>
T& Dict::get_as(const std::string& key)
{
    if (!m_data.contains(key))
    {
        m_data.mutable_set(key, T());
    }

    Value& value = m_data.mutable_get(key);

    try
    {
        return std::get<T>(value);
    }
    catch (const std::bad_variant_access&)
    {
        throw std::runtime_error("Dict value type mismatch.");
    }
}

} // namespace core
