#pragma once

#include "core/value/value.h"

#include "foundation/immutable/map.h"

#include <cstdint>
#include <string>

namespace core
{

class Dict
{
  public:
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
