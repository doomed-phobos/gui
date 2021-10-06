#ifndef _OS_EVENT_HPP
#define _OS_EVENT_HPP
#include "gfx/point.hpp"
#include "gfx/rect.hpp"
#include "os/keys.hpp"
#include "base/paths.hpp"

namespace os
{
   struct MouseEvent
   {
      enum Button {
         kNoneButton,
         kLeftButton,
         kMiddleButton,
         kRightButton,
         kUnknownButton
      };

      MouseEvent() : button(kNoneButton) {}

      Button button;
      gfx::Point position;
      gfx::Point delta;
      KeyModifiers modifiers;
   };

   struct KeyEvent
   {
      KeyEvent() : modifiers(kKeyNoneModifier),
                   repeat(0),
                   keyCode(kKeyNil),
                   unicodeChar(0),
                   isDeadKey(false) {}

      KeyModifiers modifiers;
      int repeat; // !<Cero significa que es la primera vez que se presionó
      KeyCode keyCode;
      int unicodeChar;
      bool isDeadKey;
   };

   struct ResizeEvent
   {
      gfx::Size newSize;
   };

   struct DropFilesEvent
   {
      base::paths_t files;
   };
} // namespace os
#endif