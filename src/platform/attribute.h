#pragma once

#include "platform/value.h"

namespace platform
{

class Attribute
{
  public:
    static constexpr float default_time = 0.0f;

    Attribute() = default;

    template <typename T>
    const T& get(float time = default_time) const
    {
        // Placeholder until good implementation is provided.
        return m_value.get<T>();
    }

    const Value& get(float time = default_time) const
    {
        return m_value;
    }

  private:
    Value m_value;
};

} // namespace platform
