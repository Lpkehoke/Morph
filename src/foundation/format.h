#pragma once

#include <cstddef>
#include <cstdio>
#include <string>
#include <utility>


namespace foundation
{

template <typename... Args>
std::string format(const std::string& fmt, Args&&... args)
{
    const std::size_t buf_size = std::snprintf(nullptr, 0, fmt.c_str(), std::forward<Args>(args)...);
    std::string buffer(buf_size, '\0');

    // TODO: implement error checking.
    std::snprintf(buffer.data(), buf_size, fmt.c_str(), std::forward<Args>(args)...);

    return buffer;
}

} // namespace foundation
