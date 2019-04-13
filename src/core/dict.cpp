#include "core/dict.h"

#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace core
{

Dict::Value& Dict::operator[](const std::string& key)
{
    if (!m_data.contains(key))
    {
        m_data.mutable_set(key, std::monostate{});
    }

    return m_data.mutable_get(key);
}

const Dict::Value& Dict::operator[](const std::string& key) const
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

std::string Dict::to_string() const
{
    std::ostringstream os;
    os << *this;
    return os.str();
}

std::ostream& operator<<(std::ostream &os, const Dict &dict)
{
    os << "{";

    auto itr = dict.begin();

    while (itr != dict.end())
    {
        auto& pair = *itr;

        os << pair.first << ": ";
        std::visit(
            [&](const auto& v)
            {
                constexpr bool is_none = std::is_same<
                    typename std::decay<decltype(v)>::type,
                    std::monostate>::value;
                
                constexpr bool is_bool = std::is_same<
                    typename std::decay<decltype(v)>::type,
                    bool>::value;

                if constexpr (is_none)
                {
                    os << "None";
                    return;
                }
                
                if constexpr (is_bool)
                {
                    os << (v ? "True" : "False");
                    return;
                }
                
                if constexpr (!is_none)
                {
                    os << v;
                }
            },
            pair.second);

        ++itr;

        if (itr != dict.end())
        {
            os << ", ";
        }
    }

    os << "}";

    return os;
}

} // namespace core