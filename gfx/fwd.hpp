#ifndef _GFX_FWD_HPP
#define _GFX_FWD_HPP

namespace gfx
{
   template<typename T> class PointT;
   template<typename T> class SizeT;
   template<typename T> class RectT;

   typedef PointT<int> Point; typedef PointT<float> PointF;
   typedef SizeT<int> Size; typedef SizeT<float> SizeF;
   typedef RectT<int> Rect; typedef RectT<float> RectF;
} // namespace gfx

#endif