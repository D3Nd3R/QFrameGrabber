#pragma once

#include <cstdint>
#include <string>
#include <variant>

namespace frame_grabber
{
using InputInfo = std::variant<int32_t, std::string>;
} // namespace frame_grabber