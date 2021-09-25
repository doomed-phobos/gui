#ifndef _OS_WIN_WIN_CONVERT_HPP
#define _OS_WIN_WIN_CONVERT_HPP
#include "gfx/rect.hpp"
#include "os/window_style.hpp"

#include <windows.h>

namespace os
{
   inline gfx::Rect from_win32(const RECT& rc)
   {
      return gfx::Rect(rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
   }

   inline DWORD to_win32(WindowStyle style)
   {
      auto hasStyle = [style](WindowStyle s) -> bool {
         return ((style & s) == s);
      };

      DWORD winStyle = 0;

      if(style == 0) {
         winStyle |= WS_POPUP;
      } else {
         if(hasStyle(kTitleBar_WindowStyle))    winStyle |= WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
         if(hasStyle(kMinimizable_WindowStyle)) winStyle |= WS_SYSMENU | WS_MINIMIZEBOX;
         if(hasStyle(kMaximizable_WindowStyle)) winStyle |= WS_MAXIMIZEBOX;
         if(hasStyle(kResize_WindowStyle))      winStyle |= WS_THICKFRAME;
      }

      return winStyle;
   }
} // namespace os

#endif