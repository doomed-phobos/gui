#ifndef _GFX_RECT_HPP
#define _GFX_RECT_HPP
#include "base/base.hpp"

#include <SkRect.h>

namespace gfx
{
   template<typename T>
   class PointT;

   template<typename T>
   class SizeT;
   
   template<typename T>
   class RectT
   {
   public:
      T x, y;
      T w, h;

      RectT() : x(0), y(0) ,
                w(0), h(0) {}
      RectT(const T& x, const T& y, const T& w, const T& h) : x(x), y(y),
                                                                       w(w), h(h) {}
      RectT(const PointT<T>& pt, const SizeT<T>& sz) : x(pt.x), y(pt.y),
                                                       w(sz.w), h(sz.h) {}
      RectT(PointT<T> leftTop, PointT<T> rightBottom) {
         T temp;
         
         if(leftTop.x > rightBottom.x) {
            temp = leftTop.x;
            leftTop.x = rightBottom.x;
            rightBottom.x = temp;
         }

         if(leftTop.y > rightBottom.y) {
            temp = leftTop.y;
            leftTop.y = rightBottom.y;
            rightBottom.y = temp;
         }

         x = leftTop.x;
         y = leftTop.y;
         w = rightBottom.x - leftTop.x;
         h = rightBottom.y - leftTop.y;
      }
      RectT(const SkRect& skRect) :
         x(static_cast<T>(skRect.x())),
         y(static_cast<T>(skRect.y())),
         w(static_cast<T>(skRect.width())),
         h(static_cast<T>(skRect.height())) {}

      template<typename U,
               BASE_ENABLE_IF(std::is_convertible_v<T, U>)>
      RectT(const RectT<U>& rc) : x(static_cast<T>(rc.x)), y(static_cast<T>(rc.y)),
                                           w(static_cast<T>(rc.w)), h(static_cast<T>(rc.h)) {}

      bool isEmpty() const {
         return w < 0 || h < 0;
      }

      bool contains(const PointT<T>& pt) const {
         return pt.x >= x && pt.x < x+w &&
                pt.y >= y && pt.y < y+h;
      }

      bool intersects(const RectT& rc) const {
         if(isEmpty() || rc.isEmpty())
            return false;
         
         return
            rc.x < x+w && rc.x+rc.w > x &&
            rc.y < y+h && rc.y+rc.h > y;
      }

      PointT<T> center() const {
         return PointT<T>(x+w/2, y+h/2);
      }

      PointT<T> origin() const {
         return PointT<T>(x, y);
      }
      
      PointT<T> point2() const {
         return PointT<T>(x+w, y+h);
      }

      SizeT<T> size() const {
         return SizeT<T>(w, h);
      }

      RectT& setOrigin(const T& x, const T& y) {
         this->x = x;
         this->y = y;

         return *this;
      }

      RectT& setOrigin(const PointT<T>& pt) {
         return setOrigin(pt.x, pt.y);
      }

      RectT& setSize(const T& w, const T& h) {
         this->w = w;
         this->h = h;

         return *this;
      }

      RectT& setSize(const SizeT<T>& sz) {
         return setSize(sz.w, sz.h);
      }

      RectT& offset(const T& dx, const T& dy) {
         x += dx;
         y += dy;
         return *this;
      }

      RectT& offset(const PointT<T>& delta) {
         x += delta.x;
         y += delta.y;
         return *this;
      }

      RectT& shrink(const T& unit) {
         x += unit;
         y += unit;
         w -= unit*2;
         h -= unit*2;
         return *this;
      }

      RectT createUnion(const RectT& rc) const {
         if(isEmpty())
            return rc;
         else if(rc.isEmpty())
            return *this;
         else
            return RectT(PointT<T>(x < rc.x ? x : rc.x,
                                   y < rc.y ? y : rc.y),
                         PointT<T>(x+w > rc.x+rc.w ? x+w : rc.x+rc.w,
                                   y+h > rc.y+rc.h ? y+h : rc.y+rc.h));
      }

      RectT createIntersection(const RectT& rc) const {
         if(intersects(rc))
            return RectT(PointT<T>(x > rc.x ? x : rc.x,
                                   y > rc.y ? y : rc.y),
                         PointT<T>(x+w < rc.x+rc.w ? x+w : rc.x+rc.w,
                                   y+h < rc.y+rc.h ? y+h : rc.y+rc.h));
         else
            return RectT();
      }

      operator SkRect() const {
         return SkRect::MakeXYWH(
            static_cast<float>(x),
            static_cast<float>(y),
            static_cast<float>(w),
            static_cast<float>(h)
         );
      }

      RectT operator*(const SizeT<T>& sz) {
         return RectT(x, y, w*sz.w, h*sz.h);
      }
      RectT operator/(const SizeT<T>& sz) {
         return RectT(x, y, w/sz.w, h/sz.h);
      }

      RectT& operator*=(const T& value) {
         x *= value;
         y *= value;
         w *= value;
         h *= value;

         return *this;
      }
      RectT& operator/=(const T& value) {
         x /= value;
         y /= value;
         w /= value;
         h /= value;
         
         return *this;
      }
      RectT& operator|=(const RectT& rc) {
         return *this = createUnion(rc);
      }

      bool operator==(const RectT& other) const {
         return x == other.x && y == other.y &&
                w == other.w && h == other.h;
      }
      bool operator!=(const RectT& other) const {
         return x != other.x || y != other.y ||
                w != other.w || h != other.h;
      }
   };

   typedef RectT<int> Rect;
   typedef RectT<float> RectF;
} // namespace gfx
#endif