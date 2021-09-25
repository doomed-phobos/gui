#include "os/surface.hpp"

#include "os/skia/skia_convert.hpp"
#include "base/exception.hpp"
#include "gfx/point.hpp"
#include "gfx/rect.hpp"
#include "gfx/size.hpp"

#include "SkCanvas.h"
#include "SkImage.h"
#include "SkPaint.h"
#include "SkTextBlob.h"
#include "SkTextUtils.h"

#include "src/core/SkGlyphRun.h"

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

   void Surface::drawString(const std::string& str, float x, float y,
                            const SkPaint& paint, const SkFont& font)
   {
      static SkGlyphRunBuilder glyphBuilder;
      // auto glyphList = glyphBuilder.textToGlyphRunList(font, paint, str.c_str(), str.size(), {x, y});
      SkTextBlob::Make

      m_canvas->drawString(str.c_str(), x, y, font, paint);
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
} // namespace os