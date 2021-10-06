#ifndef _OS_WINDOW_STYLE_HPP
#define _OS_WINDOW_STYLE_HPP

namespace os
{
   enum WindowStyle : unsigned
   {
      kTitleBar_WindowStyle    = 1 << 0,
      kMinimizable_WindowStyle = 1 << 1,
      kMaximizable_WindowStyle = 1 << 2,
      kResize_WindowStyle      = 1 << 3,
      kFloating_WindowStyle    = 1 << 4,
      kPopup_WindowStyle       = 1 << 5,
      kDefault_WindowStyle     = kTitleBar_WindowStyle | kResize_WindowStyle | kMinimizable_WindowStyle | kMaximizable_WindowStyle
   };
} // namespace os

#endif