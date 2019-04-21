#include "core/value/defaultvalues.h"

#include "core/dict.h"

#include <cstdint>
#include <utility>

namespace core
{

const ValueType bool_value_type(
    "Boolean",
    ValueType::TypeTag<bool> {},
    ValueType::DefCastsFrom<
        int,
        short,
        long,
        long long,
        std::int16_t,
        std::int32_t,
        std::int64_t,
        unsigned int,
        unsigned short,
        unsigned long,
        unsigned long long,
        std::uint16_t,
        std::uint32_t,
        std::uint64_t> {});

const ValueType integer_value_type(
    "Integer",
    ValueType::TypeTag<std::int64_t> {},
    ValueType::DefCastsFrom<
        int,
        short,
        long,
        long long,
        std::int16_t,
        std::int32_t> {});

const ValueType unsigned_value_type(
    "Unsigned",
    ValueType::TypeTag<std::uint64_t> {},
    ValueType::DefCastsFrom<
        unsigned int,
        unsigned short,
        unsigned long,
        unsigned long long,
        std::uint16_t,
        std::uint32_t> {});

const ValueType float_value_type(
    "Float",
    ValueType::TypeTag<float> {},
    ValueType::DefCastsFrom<double> {});

const ValueType dict_value_type(
    "Dict",
    ValueType::TypeTag<Dict> {});

const ValueType string_value_type(
    "String",
    ValueType::TypeTag<std::string> {},
    ValueType::DefCastsFrom<char*> {});

Value& operator<<(Value& value, bool new_value)
{
    value = Value(std::move(new_value), bool_value_type);
    return value;
}

Value& operator<<(Value& value, std::int64_t new_value)
{
    value = Value(std::move(new_value), integer_value_type);
    return value;
}

Value& operator<<(Value& value, std::uint64_t new_value)
{
    value = Value(std::move(new_value), unsigned_value_type);
    return value;
}

Value& operator<<(Value& value, float new_value)
{
    value = Value(std::move(new_value), float_value_type);
    return value;
}

Value& operator<<(Value& value, const Dict& new_value)
{
    value = Value(new_value, dict_value_type);
    return value;
}

Value& operator<<(Value& value, Dict&& new_value)
{
    value = Value(std::move(new_value), dict_value_type);
    return value;
}

Value& operator<<(Value& value, const std::string& new_value)
{
    value = Value(new_value, string_value_type);
    return value;
}

Value& operator<<(Value& value, std::string&& new_value)
{
    value = Value(std::move(new_value), string_value_type);
    return value;
}

} // namespace core
