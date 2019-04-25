#pragma once

#include "core/value/defaultvalues.h"
#include "core/value/valuetype.h"

#include <stdexcept>
#include <utility>

namespace core
{

class Value
{
  public:
    Value();
    Value(const Value& other);
    Value(Value&& other);

    template <typename T>
    Value(const T& value, const ValueType& val_type)
    {
        if (!val_type.is<T>())
        {
            throw std::runtime_error("Type mismatch.");
        }

        m_held = new ValueHolder<T>(value, val_type);
    }

    template <typename T>
    Value(T&& value, const ValueType& val_type)
    {
        if (!val_type.is<T>())
        {
            throw std::runtime_error("Type mismatch.");
        }

        m_held = new ValueHolder<T>(std::move(value), val_type);
    }

    Value& operator=(const Value& other);
    Value& operator=(Value&& other);

    template <typename T>
    Value& operator=(T&& other);

    ~Value();

    template <typename T>
    T& cast()
    {
        return cast_impl<T>();
    }

    template <typename T>
    const T& cast() const
    {
        return cast_impl<T>();
    }

    const ValueType& value_type() const;

    bool operator==(const Value& other) const;

  private:
    struct ValueHolderBase
    {
        virtual const ValueType& value_type() const = 0;
        virtual ValueHolderBase* clone() const = 0;
        virtual bool operator==(const ValueHolderBase& other) const = 0;
        virtual void try_cast_from(std::size_t type_hash_code, const void* arg) = 0;
        virtual ~ValueHolderBase() = default;

        template <typename T>
        bool is() const
        {
            return value_type().is<T>();
        }

        template <typename T>
        bool can_cast_from() const
        {
            return value_type().can_cast_from<T>();
        }
    };

    template <typename T>
    struct ValueHolder : public ValueHolderBase
    {
        ValueHolder(const T& value, const ValueType& val_type)
          : m_value(value)
          , m_value_type(val_type)
        {}

        ValueHolder(T&& value, const ValueType& val_type)
          : m_value(std::move(value))
          , m_value_type(val_type)
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

        virtual bool operator==(const ValueHolderBase& other) const override
        {
            if (other.value_type() != m_value_type)
            {
                return false;
            }

            return m_value_type.equals(static_cast<const ValueHolder&>(other).m_value, m_value);
        }

        virtual void try_cast_from(std::size_t type_hash_code, const void* arg) override
        {
            m_value = *m_value_type.try_cast_from<T>(type_hash_code, arg);
        }

        T                   m_value;
        const ValueType&    m_value_type;
    };

    template <typename T>
    T& cast_impl() const
    {
        if (!m_held)
        {
            throw std::runtime_error("Value is uninitialized.");
        }

        if (m_held->is<T>())
        {
            ValueHolder<T>* held = static_cast<ValueHolder<T>*>(m_held);
            return held->m_value;
        }
        
        throw std::runtime_error("Value type mismatch.");
    }
    
    ValueHolderBase* m_held;
};


//
//  Value implementation.
//

template <typename T>
Value& Value::operator=(T&& other)
{
    if (!m_held)
    {
        return *this << std::forward<T>(other);
    }
    
    m_held->try_cast_from(typeid(T).hash_code(), &other);
    return *this;
}

} // namespace core
