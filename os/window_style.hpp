#ifndef _OS_WINDOW_STYLE_HPP
#define _OS_WINDOW_STYLE_HPP

namespace os
{
   enum WindowStyle : unsigned
   {
      kNone_WindowStyle        = 0,
      kTitleBar_WindowStyle    = 1 << 0,
      kMinimizable_WindowStyle = 1 << 1,
      kMaximizable_WindowStyle = 1 << 2,
      kResize_WindowStyle      = 1 << 3,
      kDefault_WindowStyle     = kTitleBar_WindowStyle | kResize_WindowStyle | kMinimizable_WindowStyle | kMaximizable_WindowStyle
   };
} // namespace os

#endif