#ifndef _GFX_FWD_HPP
#define _GFX_FWD_HPP

namespace gfx
{
   template<typename T> class PointT;
   template<typename T> class SizeT;
   template<typename T> class RectT;
   template<typename T> class ClipT;

   typedef PointT<int> Point; typedef PointT<float> PointF;
   typedef SizeT<int> Size; typedef SizeT<float> SizeF;
   typedef RectT<int> Rect; typedef RectT<float> RectF;
   typedef ClipT<int> Clip; typedef ClipT<float> ClipF;
} // namespace gfx

#endif