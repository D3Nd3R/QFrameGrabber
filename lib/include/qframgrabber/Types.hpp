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

struct SharedMat
{
    std::string sharedMemory;
};

using InputInfo = std::variant<std::monostate, int32_t, Url, SharedMat>;
} // namespace frame_grabber