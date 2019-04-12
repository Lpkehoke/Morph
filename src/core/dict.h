#pragma once

#include "foundation/immutable/map.h"

#include <string>
#include <variant>

namespace core
{

class Dict
{
  public:
    using Value = std::variant<std::monostate, Dict, std::string, double, bool>;
    using Iterator = foundation::Map<std::string, Value>::Iterator;

    Value& operator[](const std::string& key);
    const Value& operator[](const std::string& key) const;

    void update(const Dict& other);

    Iterator begin() const;
    Iterator end() const;

    bool contains(const std::string& key) const;
    bool operator==(const Dict& other) const;

  private:
    foundation::Map<std::string, Value> m_data;
};

} // namespace core
