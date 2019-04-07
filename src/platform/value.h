#pragma once

#include <cstddef>
#include <stdexcept>
#include <typeinfo>
#include <utility>

namespace platform
{

class Value
{
  public:
    Value();
    Value(const Value& other);
    Value(Value&& other);

    template <typename T>
    Value(const T& value)
      : m_held(new ValueHolder<T>(value))
    {}

    template <typename T>
    Value(T&& value)
      : m_held(new ValueHolder<T>(std::move(value)))
    {}

    ~Value();

    template <typename T>
    T& get()
    {
        return get_impl<T>();
    }

    template <typename T>
    const T& get() const
    {
        return get_impl<T>();
    }

  private:
    struct ValueHolderBase
    {
        virtual std::size_t type_hash_code() const = 0;
        virtual ValueHolderBase* clone() const = 0;

        virtual ~ValueHolderBase() = default;

        template <typename T>
        bool is() const
        {
            return typeid(T).hash_code() == type_hash_code();
        }
    };

    template <typename T>
    struct ValueHolder : public ValueHolderBase
    {
        ValueHolder(const T& value)
          : m_value(value)
          , m_hash_code(typeid(T).hash_code())
        {}

        ValueHolder(T&& value)
          : m_value(std::move(value))
          , m_hash_code(typeid(T).hash_code())
        {}

        ValueHolder(const ValueHolder& other)
          : m_value(other.m_value)
          , m_hash_code(other.m_hash_code)
        {}

        virtual std::size_t type_hash_code() const override
        {
            return m_hash_code;
        }

        virtual ValueHolderBase* clone() const override
        {
            return new ValueHolder(*this);
        }

        T                 m_value;
        const std::size_t m_hash_code;
    };

    template <typename T>
    T& get_impl() const
    {
        if (!m_held)
        {
            throw std::runtime_error("Value is uninitialized.");
        }

        if (!m_held->is<T>())
        {
            throw std::runtime_error("Value type mismatch.");
        }

        ValueHolder<T>* held = static_cast<ValueHolder<T>*>(m_held);
        return held->m_value;
    }

    ValueHolderBase* m_held;
};

} // namespace platform
