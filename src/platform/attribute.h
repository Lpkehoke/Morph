#pragma once

#include <cstdint>
#include <variant>
#include <utility>

namespace platform
{

class Attribute
{
  public:
    Attribute();
    Attribute(const Attribute& other);

    template <typename T>
    T& cast();

    template <typename T>
    const T& cast() const;

  private:
    using Value = std::variant<std::int64_t, std::uint64_t, double>;

    Value m_value;
};

} // namespace platform
