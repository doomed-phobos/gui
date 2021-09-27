#ifndef _BASE_BASE_HPP
#define _BASE_BASE_HPP
#include <type_traits>

#define BASE_ENABLE_IF(...) std::enable_if_t<(__VA_ARGS__), int> = 0
#define DISALLOW_COPYING(classname)                \
   classname(const classname&) = delete;           \
   classname& operator=(const classname&) = delete;

#define DISALLOW_MOVING(classname)            \
   classname(classname&&) = delete;           \
   classname& operator=(classname&&) = delete;

namespace base
{
   template<typename T>
   static constexpr inline const T& clamp(const T& x, const T& low, const T& high)
   {
      return std::max<T>(low, std::min<T>(x, high));
   }

   template<typename T>
   static constexpr inline std::enable_if_t<std::is_arithmetic_v<T>, T> abs(const T& val)
   {
      return val < T() ? -val : val;
   }

   template<typename T>
   static constexpr inline std::enable_if_t<std::is_arithmetic_v<T>, T> sign(const T& val)
   {
      return val < T() ? -1 : 1;
   }
} // namespace base

#endif