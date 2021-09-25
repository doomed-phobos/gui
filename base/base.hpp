#ifndef _BASE_BASE_HPP
#define _BASE_BASE_HPP
#include <type_traits>

#define BASE_ENABLE_IF(...) std::enable_if_t<(__VA_ARGS__), int> = 0

#endif