#ifndef _OS_WINDOW_VISIBLE_MODE_HPP
#define _OS_WINDOW_VISIBLE_MODE_HPP

namespace os
{
   /// @enum WindowBase::WindowVisibleMode
   /// Describes how to show a window.
   enum class WindowVisibleMode : unsigned
   {
      kShow,         /// !< Show window normally but don't focus it
      kShowAndFocus, /// !< Show window normally and focus it
      kHide,         /// !< Hide window
   };
} // namespace os


#endif