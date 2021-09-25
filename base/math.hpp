#ifndef _BASE_MATH_HPP
#define _BASE_MATH_HPP
#include "base/types.hpp"
#include <type_traits>

namespace base
{
   template<typename T>
   static constexpr std::enable_if_t<std::is_arithmetic_v<T>, T> abs(const T& val)
   {
      return val < T() ? -val : val;
   }

   template<typename T>
   static constexpr std::enable_if_t<std::is_arithmetic_v<T>, T> sign(const T& val)
   {
      return val < T() ? -1 : 1;
   }
} // namespace base
#endif