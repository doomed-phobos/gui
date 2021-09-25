#ifndef _GFX_SIZE_HPP
#define _GFX_SIZE_HPP
#include "base/base.hpp"

namespace gfx
{
   template<typename T>
   class PointT;

   template<typename T>
   class SizeT
   {
   public:
      T w, h;

      SizeT() : w(0), h(0) {}
      SizeT(const T& w, const T& h) : w(w), h(h) {}
      template<typename U,
         BASE_ENABLE_IF(std::is_convertible_v<T, U>)>
      SizeT(const SizeT<U>& sz) : w(static_cast<T>(sz.w)),
                                  h(static_cast<T>(sz.h)) {}
      explicit SizeT(const PointT<T>& pt) : w(pt.x), h(pt.y) {}

      SizeT operator+(const T& value) const {
         return SizeT(w+value, h+value);
      }
      SizeT operator+(const SizeT& sz) const {
         return SizeT(w+sz.w, h+sz.h);
      }
      SizeT operator*(const T& value) const {
         return SizeT(w*value, h*value);
      }
      SizeT operator/(const T& value) const {
         return SizeT(w/value, h/value);
      }
      SizeT operator%(const T& value) const {
         return SizeT(w%value, h%value);
      }
      SizeT operator-(const SizeT& sz) const {
         return SizeT(w - sz.w, h-sz.h);
      }

      SizeT& operator+=(const SizeT& sz) {
         w += sz.w;
         h += sz.h;
         return *this;
      }
      SizeT& operator-=(const SizeT& sz) {
         w -= sz.w;
         h -= sz.h;
         return *this;
      }
      SizeT& operator=(const SizeT& sz) {
         w = sz.w;
         h = sz.h;
         return *this;
      }

      bool operator>(const SizeT& sz) const {
         return w > sz.w && h > sz.h;
      }
      bool operator>(const T& value) const {
         return w > value && h > value;
      }
      bool operator>=(const SizeT& sz) const {
         return w >= sz.w &&
                h >= sz.h;
      }
      bool operator>=(const T& value) const {
         return w >= value &&
                h >= value;
      }
      bool operator<=(const SizeT& sz) const {
         return w <= sz.w &&
                h <= sz.h;
      }
      bool operator<=(const T& value) const {
         return w <= value &&
                h <= value;
      }
      bool operator!=(const SizeT& sz) const {
         return w != sz.w && h != sz.h;
      }
      bool operator!=(const T& value) const {
         return w != value && h != value;
      }
   };

   typedef SizeT<int> Size;
   typedef SizeT<float> SizeF;
} // namespace gfx
#endif