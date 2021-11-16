#ifndef _GFX_CLIP_HPP
#define _GFX_CLIP_HPP
#include "gfx/point.hpp"
#include "gfx/size.hpp"
#include "gfx/rect.hpp"

namespace gfx
{
   template<typename T>
   class ClipT
   {
   public:
      PointT<T> dst;
      PointT<T> src;
      SizeT<T> size;

      ClipT() :
         ClipT(T(0), T(0),
                T(0), T(0),
                T(0), T(0)) {}
      ClipT(T w, T h) :
         ClipT(T(0), T(0),
                T(0), T(0),
                w, h) {}

      ClipT(T dstX, T dstY, const RectT<T>& srcBounds) :
         ClipT(dstX, dstY,
                srcBounds.x, srcBounds.y,
                srcBounds.w, srcBounds.h) {}
      
      ClipT(T dstX, T dstY, T srcX, T srcY, T w, T h) :
         dst(dstX, dstY),
         src(srcX, srcY),
         size(w, h) {}

      bool operator()(T avail_dstW, T avail_dstH,
                      T avail_srcW, T avail_srcH) {
         if(src.x < T(0)) {
            size.w += src.x;
            dst.x -= src.x;
            src.x = T(0);
         }

         if(src.y < T(0)) {
            size.h += src.y;
            dst.y -= src.y;
            src.y = T(0);
         }

         if(src.x + size.w > avail_srcW)
            size.w -= src.x + size.w - avail_srcW;

         if(src.y + size.h > avail_srcH)
            size.h -= src.y + size.h - avail_srcH;

         if(dst.x < T(0)) {
            size.w += dst.x;
            src.x -= dst.x;
            dst.x = T(0);
         }

         if(dst.y < T(0)) {
            size.h += dst.y;
            src.y -= dst.y;
            dst.y = T(0);
         }

         if(dst.x + size.w > avail_dstW)
            size.w -= dst.x + size.w - avail_dstW;
         
         if(dst.y + size.h > avail_dstH)
            size.h -= dst.y + size.h - avail_dstH;
         
         return size.w > T(0) && size.h > T(0); 
      }

      bool operator==(const ClipT& other) const {
         return dst == other.dst &&
                src == other.src &&
                size == other.size;
      } 
   };

   typedef ClipT<int> Clip;
   typedef ClipT<float> ClipF;
} // namespace gfx

#endif