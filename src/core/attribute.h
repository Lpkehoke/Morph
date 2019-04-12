#pragma once

#include "core/value.h"

namespace core
{

class Attribute
{
  public:
    static constexpr float default_time = 0.0f;

    Attribute(Value value);

    template <typename T>
    const T& get(float time = default_time) const;

    const Value& get(float time = default_time) const;

  private:
    Value m_value;
};


//
//  Attribute implementation.
//

template <typename T>
const T& Attribute::get(float time) const
{
    // Placeholder until good implementation is provided.
    return m_value.cast<T>();
}

} // namespace core
