#pragma once

namespace frame_grabber::meta::utils
{
/// {@
/// \brief Overload pattern for std::visit
template<class... Ts>
struct Overload : Ts...
{
    using Ts::operator()...;
};

template<class... Ts>
Overload(Ts...) -> Overload<Ts...>;
/// @}

/*!
\code{.cpp}

const std::variant<int, bool> v{true};

std::visit(
  Overload{
      [](int val) { std::cout << val; },
      [](bool val) { std::cout << std::boolalpha << val; },
  },
  v);

\endcode

 */

} // namespace videostreamer::meta::utils
