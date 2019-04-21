#pragma once

#include <cassert>
#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>


namespace core
{

namespace detail
{

using CastFn = void* (*)(const void*);
using CastsCollection = std::map<std::size_t, CastFn>;

template <typename From, typename To>
static CastFn make_cast()
{
    return [](const void* arg)
        {
            assert(arg != nullptr);
            To* result = new To(*static_cast<const From*>(arg));
            return static_cast<void*>(result);
        };
}

template <typename To, typename Head, typename... Tail>
static CastsCollection get_type_hash_codes_impl();

template <typename To, typename T>
struct GetHashCodesSingle
{
    static CastsCollection value()
    {
        return CastsCollection {{typeid(T).hash_code(), make_cast<T, To>()}};
    }
};

template <typename To, typename Head, typename... Tail>
struct GetHashCodesMultiple
{
    static CastsCollection value()
    {
        auto result = get_type_hash_codes_impl<To, Tail...>();
        result.emplace(typeid(Head).hash_code(), make_cast<Head, To>());
        return result;
    }
};


template <typename To, typename Head, typename... Tail>
static CastsCollection get_type_hash_codes_impl()
{
    using Impl = typename std::conditional<
        sizeof...(Tail) == 0,
        GetHashCodesSingle<To, Head>,
        GetHashCodesMultiple<To, Head, Tail...>>::type;

    return Impl::value();
}

template <typename To, typename... Types>
static CastsCollection get_type_hash_codes()
{
    return get_type_hash_codes_impl<To, Types...>();
}

} // namespace detail


class ValueType
{
  public:
    template <typename T>
    struct TypeTag {};

    template <typename... Types>
    struct DefCastsFrom {};

    template <typename T, typename... Types>
    ValueType(std::string name, TypeTag<T>, DefCastsFrom<Types...>);

    template <typename T>
    ValueType(std::string name, TypeTag<T>);

    template <typename T>
    ValueType(TypeTag<T>);

    ValueType(const ValueType& other);
    ValueType(ValueType&& other);

    const std::string& name() const;

    template <typename T>
    bool is() const;

    template <typename T>
    bool can_cast_from() const;

    //
    //  TODO: try_cast_from performes dynamic memory allocation, this could be avoided for type pairs
    //  that could be implicitly converted (e.g. int and bool). There should be another set of methods for such
    //  types.
    //
    template <typename T>
    std::unique_ptr<T> try_cast_from(std::size_t type_hash_code, const void* arg) const;

    bool operator==(const ValueType& other) const;
    bool operator!=(const ValueType& other) const;
  
  private:
    std::string             m_name;
    std::size_t             m_type_hash_code;
    detail::CastsCollection m_known_casts_from;
};


//
//  ValueType implementation.
//

template <typename T, typename... Types>
ValueType::ValueType(std::string name, TypeTag<T>, DefCastsFrom<Types...>)
  : m_name(std::move(name))
  , m_type_hash_code(typeid(T).hash_code())
  , m_known_casts_from(detail::get_type_hash_codes<T, Types...>())
{}

template <typename T>
ValueType::ValueType(std::string name, TypeTag<T>)
  : m_name(std::move(name))
  , m_type_hash_code(typeid(T).hash_code())
{}

template <typename T>
ValueType::ValueType(TypeTag<T>)
  : m_name(typeid(T).name())
  , m_type_hash_code(typeid(T).hash_code())
{}

template <typename T>
bool ValueType::is() const
{
    return typeid(T).hash_code() == m_type_hash_code;
}

template <typename T>
bool ValueType::can_cast_from() const
{
    if (is<T>())
    {
        return true;
    }
    
    auto itr = m_known_casts_from.find(typeid(T).hash_code());
    return itr != m_known_casts_from.end();
}

template <typename T>
std::unique_ptr<T> ValueType::try_cast_from(std::size_t type_hash_code, const void* arg) const
{
    if (!is<T>())
    {
        throw std::runtime_error("T should match the type of this ValueType.");
    }

    auto cast_itr = m_known_casts_from.find(type_hash_code);
    if (cast_itr == m_known_casts_from.end())
    {
        throw std::runtime_error("Can't cast from this type.");
    }

    auto cast_fn = cast_itr->second;
    return std::unique_ptr<T>(static_cast<T*>((*cast_fn)(&arg)));
}

} // namespace core
