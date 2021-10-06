#ifndef _OS_SURFACE_HPP
#define _OS_SURFACE_HPP
#include "os/ref.hpp"
#include "os/fwd.hpp"
#include "os/text_align.hpp"
#include "gfx/fwd.hpp"

#include <SkBitmap.h>

class SkCanvas;
class SkPath;
class SkPaint;
class SkFont;

namespace os
{
   class Surface : public Referenceable
   {
   public:
      Surface(const SkBitmap& bmp);
      Surface(int width, int height);
      ~Surface();

      void createRGBA(int width, int height);
      void swapBitmap(SkBitmap& other);

      uint8_t* getData(int x, int y) const;

      void clear(const SkColor& c);
      // TODO: Write Matrix
      SkColor getPixel(int x, int y) const;
      SkColor getPixel(const SkPoint& pt) const;
      void putPixel(int x, int y, const SkColor& c);

      void drawLine(const float x0, const float y0,
                    const float x1, const float y1, const SkPaint& paint);
      void drawLine(const SkPoint& pt0, const SkPoint& pt1,
                    const SkPaint& paint);

      void drawCircle(const float cx, const float cy, const float radius,
                      const SkPaint& paint);
      void drawCircle(const SkPoint& center, const float radius,
                      const SkPaint& paint);

      void drawRect(const float x, const float y,
                    const float w, const float h, const SkPaint& paint);
      void drawRect(const SkRect& rc, const SkPaint& paint);
      void drawRoundRect(const float x, const float y,
                         const float w, const float h,
                         const float rx, const float ry, const SkPaint& paint);
      void drawRoundRect(const SkRect& rc,
                         const float rx, const float ry, const SkPaint& paint);
      void drawPath(const SkPath& path, const SkPaint& paint);
      void drawString(const std::string_view& str, float x, float y,
                      const SkPaint& paint, const SkFont& font, const TextAlign& align = kLeft_TextAlign);
      void drawString(const std::string_view& str, const SkPoint& position,
                      const SkPaint& paint, const SkFont& font,
                      const TextAlign& align = kLeft_TextAlign);
      
      void blitTo(Surface* dst, int srcX, int srcY,
                  int dstX, int dstY, int width, int height);
      void drawRGBASurface(const Surface* src, int dstX, int dstY);

      const SkBitmap& skBitmap() const;
      int width() const;
      int height() const;
      gfx::Size size() const;
      gfx::Rect clipBounds() const;
      gfx::Rect bounds() const;
   private:
      void build();

      SkCanvas* m_canvas;
      SkBitmap m_bitmap;
   };

   typedef SharedPtr<Surface> SurfacePtr;
} // namespace os

#endif