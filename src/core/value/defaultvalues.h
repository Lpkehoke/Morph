#pragma once

#include "core/value/valuetype.h"

#include <cstdint>
#include <string>

namespace core { class Dict; }
namespace core { class Value; }

namespace core
{

extern const ValueType bool_value_type;
extern const ValueType integer_value_type;
extern const ValueType unsigned_value_type;
extern const ValueType float_value_type;
extern const ValueType string_value_type;
extern const ValueType dict_value_type;

Value& operator<<(Value& value, bool new_value);
Value& operator<<(Value& value, std::int64_t new_value);
Value& operator<<(Value& value, std::uint64_t new_value);
Value& operator<<(Value& value, float new_value);
Value& operator<<(Value& value, const std::string& new_value);
Value& operator<<(Value& value, std::string&& new_value);
Value& operator<<(Value& value, const Dict& new_value);
Value& operator<<(Value& value, Dict&& new_value);

    
} // namespace core
