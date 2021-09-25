#ifndef _GFX_POINT_IO_HPP
#define _GFX_POINT_IO_HPP
#include "gfx/point.hpp"

#include <iosfwd>

namespace gfx
{
   template<typename T>
   inline std::ostream& operator<<(std::ostream& os, const PointT<T>& point)
   {
      return os << "("
               << point.x << ", "
               << point.y << ")";
   }

   template<typename T>
   inline std::istream& operator>>(std::istream& in, PointT<T>& point)
   {
      while (in && in.get() != '(');

      if (!in)
         return in;

      char chr;
      in >> point.x >> chr
         >> point.y;

      return in;
   }
} // namespace gfx
#endif