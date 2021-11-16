#include "os/surface.hpp"

#include "os/skia/skia_convert.hpp"
#include "base/exception.hpp"
#include "gfx/point.hpp"
#include "gfx/rect.hpp"
#include "gfx/size.hpp"
#include "gfx/clip.hpp"

#include <SkImage.h>
#include <SkPaint.h>
#include <SkTextBlob.h>
#include <SkTextUtils.h>

namespace os
{  
   Surface::Surface(const SkBitmap& bmp) :
      m_bitmap(bmp),
      m_canvas(nullptr)
   {
      this->build();
   }

   Surface::Surface(int width, int height) :
      m_canvas(nullptr)
   {
      this->createRGBA(width, height);
   }

   Surface::~Surface()
   {
      if(m_canvas)
         delete m_canvas;
   }

   void Surface::createRGBA(int width, int height)
   {
      if(!m_bitmap.tryAllocPixels(SkImageInfo::MakeN32Premul(width, height)))
         throw base::Exception("Error to trying allocate pixels!");

      m_bitmap.eraseColor(SK_ColorTRANSPARENT);
      build();
   }

   void Surface::swapBitmap(SkBitmap& other)
   {
      m_bitmap.swap(other);
      build();
   }

   void Surface::build()
   {
      delete m_canvas;
      m_canvas = new SkCanvas(m_bitmap);
   }

   uint8_t* Surface::getData(int x, int y) const
   {
      if(m_bitmap.isNull())
         return nullptr;
      else
         return (uint8_t*)m_bitmap.getAddr32(x, y);
   }

   void Surface::clear(const SkColor& c)
   {
      m_canvas->clear(c);
   }

   SkColor Surface::getPixel(int x, int y) const
   {
      return m_bitmap.getColor(x, y);
   }

   SkColor Surface::getPixel(const SkPoint& pt) const
   {
      return getPixel((int)pt.x(), (int)pt.y());
   }

   void Surface::putPixel(int x, int y, const SkColor& c)
   {
      SkPaint p;
      p.setColor(c);
      m_canvas->drawPoint(SkIntToScalar(x), SkIntToScalar(y), p);
   }

   void Surface::drawLine(const float x0, const float y0,
                          const float x1, const float y1, const SkPaint& paint)
   {
      m_canvas->drawLine(x0, y0, x1, y1, paint);
   }

   void Surface::drawLine(const SkPoint& pt0, const SkPoint& pt1,
                          const SkPaint& paint)
   {
      drawLine(pt0.x(), pt0.y(), pt1.x(), pt1.y(), paint);
   }

   void Surface::drawCircle(const float cx, const float cy, const float radius,
                            const SkPaint& paint)
   {
      m_canvas->drawCircle(cx, cy, radius, paint);
   }

   void Surface::drawCircle(const SkPoint& center, const float radius,
                            const SkPaint& paint)
   {
      drawCircle(center.x(), center.y(), radius, paint);
   }

   void Surface::drawRect(const float x, const float y,
                          const float w, const float h, const SkPaint& paint)
   {
      drawRect(SkRect::MakeXYWH(x, y, w, h), paint);
   }

   void Surface::drawRect(const SkRect& rc, const SkPaint& paint)
   {
      if(rc.isEmpty()) return;

      m_canvas->drawRect(rc, paint);
   }

   void Surface::drawRoundRect(const float x, const float y,
                               const float w, const float h,
                               const float rx, const float ry, const SkPaint& paint)
   {
      drawRoundRect(SkRect::MakeXYWH(x, y, w, h), rx, ry, paint);
   }

   void Surface::drawRoundRect(const SkRect& rc,
                               const float rx, const float ry, const SkPaint& paint)
   {
      if(rc.isEmpty()) return;

      m_canvas->drawRoundRect(rc, rx, ry, paint);
   }

   void Surface::drawPath(const SkPath& path, const SkPaint& paint)
   {
      m_canvas->drawPath(path, paint);
   }

   void Surface::drawString(const std::string_view& str, float x, float y,
                            const SkPaint& paint, const SkFont& font, const TextAlign& align)
   {
      SkTextUtils::Draw(
         m_canvas,
         str.data(),
         str.size(),
         SkTextEncoding::kUTF8,
         x, y,
         font, paint,
         to_skia(align)
      );
   }

   void Surface::drawString(const std::string_view& str, const SkPoint& position,
                            const SkPaint& paint, const SkFont& font,
                            const TextAlign& align)
   {
      this->drawString(str, position.x(), position.y(), paint, font, align);
   }

   void Surface::blitTo(Surface* dst, int srcX, int srcY,
                            int dstX, int dstY, int width, int height)
   {
      SkRect srcRect = SkRect::MakeXYWH(SkIntToScalar(srcX), SkIntToScalar(srcY), SkIntToScalar(width), SkIntToScalar(height));
      SkRect dstRect = SkRect::MakeXYWH(SkIntToScalar(dstX), SkIntToScalar(dstY), SkIntToScalar(width), SkIntToScalar(height));

      SkPaint p;
      p.setBlendMode(SkBlendMode::kSrc);

      if(!m_bitmap.empty()) {
         dst->m_canvas->drawImageRect(SkImage::MakeFromBitmap(m_bitmap), srcRect, dstRect,
            SkSamplingOptions::SkSamplingOptions(), &p, SkCanvas::kStrict_SrcRectConstraint);
      }
   }

   void Surface::scrollTo(const SkRect& rc, int dx, int dy)
   {
      int w = width();
      int h = height();
      
      gfx::Clip clip(rc.x()+dx, rc.y()+dy, rc);
      if(!clip(w, h, w, h))
         return;
      
      int bytesPerPixel = m_bitmap.bytesPerPixel();
      int rowBytes = (int)m_bitmap.rowBytes();
      int rowDelta;

      if(dy > 0) {
         clip.src.y += clip.size.h-1;
         clip.dst.y += clip.size.h-1;
         rowDelta = -rowBytes;
      } else
         rowDelta = rowBytes;

      char* dst = (char*)m_bitmap.getPixels();
      const char* src = dst;
      dst += rowBytes*clip.dst.y + bytesPerPixel*clip.dst.x;
      src += rowBytes*clip.src.y + bytesPerPixel*clip.dst.y;
      w = bytesPerPixel*clip.size.w;
      h = clip.size.h;

      while(-- h >= 0) {
         memmove(dst, src, w);
         dst += rowDelta;
         src += rowDelta;
      }

      m_bitmap.notifyPixelsChanged();
   }

   // FIXME: Surface no dibuja imagenes
   void Surface::drawRGBASurface(const Surface* src, int dstX, int dstY)
   {
      SkRect srcRect = SkRect::MakeXYWH(0.f, 0.f, SkIntToScalar(width()), SkIntToScalar(height()));
      SkRect dstRect = SkRect::MakeXYWH(SkIntToScalar(dstX), SkIntToScalar(dstY),
                                        SkIntToScalar(width()), SkIntToScalar(height()));

      SkPaint p;
      p.setBlendMode(SkBlendMode::kSrcOver); // TODO: RGBA

      m_canvas->drawImageRect(SkImage::MakeFromBitmap(src->skBitmap()), srcRect, dstRect,
         SkSamplingOptions::SkSamplingOptions(), &p, SkCanvas::kStrict_SrcRectConstraint);
   }

   const SkBitmap& Surface::skBitmap() const
   {
      return m_bitmap;
   }

   int Surface::width() const
   {
      return m_bitmap.width();
   }

   int Surface::height() const
   {
      return m_bitmap.height();
   }

   gfx::Size Surface::size() const
   {
      return gfx::Size(width(), height());
   }

   gfx::Rect Surface::clipBounds() const
   {
      SkIRect rc;
      if(m_canvas->getDeviceClipBounds(&rc))
         return gfx::Rect(rc.x(), rc.y(), rc.width(), rc.height());
      else
         return gfx::Rect();
   }

   gfx::Rect Surface::bounds() const
   {
      return gfx::Rect(0, 0, width(), height());
   }
} // namespace os