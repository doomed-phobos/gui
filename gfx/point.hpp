#ifndef _GFX_POINT_HPP
#define _GFX_POINT_HPP
#include "base/base.hpp"

#include <cmath>

namespace gfx
{
   template<typename T>
   class SizeT;
   
   template<typename T>
   class PointT
   {
   public:
      T x, y;

      PointT() : x(0), y(0) {}
      PointT(const T& x, const T& y) : x(x), y(y) {}
      
      template<typename U,
         BASE_ENABLE_IF(std::is_convertible_v<T, U>)>
      PointT(const PointT<U>& pt) : x(static_cast<T>(pt.x)), y(static_cast<T>(pt.y)) {}
      
      template<typename U,
               BASE_ENABLE_IF(std::is_convertible_v<T, U>)>
      explicit PointT(const SizeT<U>& sz) : x(static_cast<T>(sz.w)),
                                            y(static_cast<T>(sz.h)) {}

      PointT& operator+=(const PointT& pt) {
         x += pt.x;
         y += pt.y;

         return *this;
      }

      PointT& operator/=(const T& value) {
         x /= value;
         y /= value;

         return *this;
      }

      PointT operator+(const PointT& pt) const {
         return PointT(x + pt.x, y + pt.y);
      }

      PointT operator-(const PointT& pt) const {
         return PointT(x - pt.x, y - pt.y);
      }
      PointT operator-(const T& value) const {
         return PointT(x - value, y - value);
      }

      PointT operator/(const T& value) const {
         return PointT(x / value, y / value);
      }

      PointT operator-() const {
         return PointT(-x, -y);
      }
   };

   typedef PointT<int> Point;
   typedef PointT<float> PointF;
} // namespace gfx
#endif