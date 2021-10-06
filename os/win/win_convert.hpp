#ifndef _OS_WIN_WIN_CONVERT_HPP
#define _OS_WIN_WIN_CONVERT_HPP
#include "gfx/rect.hpp"

#include <windows.h>

namespace os
{
   inline gfx::Rect from_win32(const RECT& rc)
   {
      return gfx::Rect(rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
   }
} // namespace os

#endif