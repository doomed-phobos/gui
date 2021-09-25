#ifndef _BASE_CLAMP_HPP
#define _BASE_CLAMP_HPP
#include <algorithm>

namespace base
{
   template<typename T>
   static constexpr inline const T& clamp(const T& x, const T& low, const T& high)
   {
      return std::max<T>(low, std::min<T>(x, high));
   }
} // namespace base

#endif