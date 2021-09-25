#ifndef _OS_SKIA_SKIA_SURFACE_PAINT_HPP
#define _OS_SKIA_SKIA_SURFACE_PAINT_HPP
#include "os/surface.hpp"

#include "base/debug.hpp"

#include <windows.h>

namespace os::priv
{
   class SkiaSurfacePaint
   {
   public:
      explicit SkiaSurfacePaint(HWND handle, Surface& surface, int scale) :
         m_handle(handle),
         m_ps{0},
         m_surface(surface),
         m_scale(scale) {
         m_hDC = BeginPaint(m_handle, &m_ps);
      }

      ~SkiaSurfacePaint() {
         const SkBitmap& bitmap = m_surface.skBitmap();

         BITMAPINFO bmi = {0};
         bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
         bmi.bmiHeader.biBitCount = 32;
         bmi.bmiHeader.biWidth = bitmap.width();
         bmi.bmiHeader.biHeight = -bitmap.height();
         bmi.bmiHeader.biPlanes = 1;
         bmi.bmiHeader.biCompression = BI_RGB;

         ASSERT(bitmap.width() * bitmap.bytesPerPixel() == bitmap.rowBytes());

         StretchDIBits(m_hDC, 0, 0, bitmap.width() * m_scale, bitmap.height() * m_scale,
                        0, 0, bitmap.width(), bitmap.height(),
                        bitmap.getPixels(), &bmi, DIB_RGB_COLORS, SRCCOPY);

         EndPaint(m_handle, &m_ps);
      }
   private:
      int m_scale;
      Surface& m_surface;
      PAINTSTRUCT m_ps;
      HWND m_handle;
      HDC m_hDC;
   };
} // namespace os::priv

#endif