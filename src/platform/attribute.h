#pragma once

#include <variant>
#include <utility>

namespace platform
{

//
//  Attribute is a container for a value.
//  It's lifetime is managed by reference counting.
//
class Attribute
{
  public:
    Attribute(const Attribute& other);
    Attribute(float value);

    //
    //  If attribute value is float, returns the held value
    //  throws std::bad_variant_access otherwise.
    //
    float as_float() const;

  private:
    using Value = std::variant<float>;

    Value m_value;
};

} // namespace platform
