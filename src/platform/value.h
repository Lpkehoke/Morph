#pragma once

#include "platform/valuetype.h"

#include <stdexcept>
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
    Value(const T& value, ValueType val_type)
    {
        if (!val_type.is<T>())
        {
            throw std::runtime_error("Type mismatch.");
        }

        m_held = new ValueHolder<T>(value, std::move(val_type));
    }

    template <typename T>
    Value(T&& value, ValueType val_type)
    {
        if (!val_type.is<T>())
        {
            throw std::runtime_error("Type mismatch.");
        }

        m_held = new ValueHolder<T>(
            std::move(value),
            std::move(val_type));
    }

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

    const ValueType& value_type() const;

  private:
    struct ValueHolderBase
    {
        virtual const ValueType& value_type() const = 0;
        virtual ValueHolderBase* clone() const = 0;

        virtual ~ValueHolderBase() = default;

        template <typename T>
        bool is() const
        {
            return value_type().is<T>();
        }
    };

    template <typename T>
    struct ValueHolder : public ValueHolderBase
    {
        ValueHolder(const T& value, ValueType&& val_type)
          : m_value(value)
          , m_value_type(std::move(val_type))
        {}

        ValueHolder(T&& value, ValueType&& val_type)
          : m_value(std::move(value))
          , m_value_type(std::move(val_type))
        {}

        ValueHolder(const ValueHolder& other)
          : m_value(other.m_value)
          , m_value_type(other.m_value_type)
        {}

        virtual const ValueType& value_type() const override
        {
            return m_value_type;
        }

        virtual ValueHolderBase* clone() const override
        {
            return new ValueHolder(*this);
        }

        T           m_value;
        ValueType   m_value_type;
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
