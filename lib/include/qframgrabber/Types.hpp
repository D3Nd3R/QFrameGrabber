#pragma once

#include <cstdint>
#include <string>
#include <variant>

namespace frame_grabber
{
struct Url
{
    std::string url;
};
using InputInfo = std::variant<std::monostate, int32_t, Url>;
} // namespace frame_grabber