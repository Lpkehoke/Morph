#pragma once

#include <cstddef>
#include <string>
#include <typeinfo>
#include <utility>


namespace platform
{

class ValueType
{
  public:
    template <typename T>
    struct TypeTag {};

    template <typename T>
    ValueType(std::string name, TypeTag<T>);

    ValueType(const ValueType& other);
    ValueType(ValueType&& other);

    const std::string& name() const;

    template <typename T>
    bool is() const;

    bool operator==(const ValueType& other) const;
  
  private:
    std::string m_name;
    std::size_t m_type_hash_code;
};


//
//  ValueType implementation.
//

template <typename T>
ValueType::ValueType(std::string name, TypeTag<T>)
  : m_name(std::move(name))
  , m_type_hash_code(typeid(T).hash_code())
{}

template <typename T>
bool ValueType::is() const
{
    return typeid(T).hash_code() == m_type_hash_code;
}

} // namespace platform
