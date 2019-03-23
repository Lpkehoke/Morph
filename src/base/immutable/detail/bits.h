#pragma once

#include <cstddef>
#include <cstdint>
#ifdef _WIN32
#include <intrin.h>
#endif

namespace immutable
{
namespace detail
{

using Bitmap = std::uint64_t;
using Count  = std::uint32_t;
using Shift  = std::uint32_t;
using Hash   = std::size_t;
using Size   = std::size_t;

template <Count B, typename T = Size>
constexpr T max_depth = (sizeof(Hash) * 8u + B - 1u) / B;

template <Count B, typename T = Shift>
constexpr T max_shift = max_depth<B, T> * B;

template <Count B, typename T = Bitmap>
constexpr T mask = (1ul << B) - 1ul;

inline Count popcount(std::uint64_t x)
{
#ifdef _WIN32
	return __popcnt64(x);
#else
    return __builtin_popcountll(x);
#endif
}

} // namespace detail
} // namespace immutable
