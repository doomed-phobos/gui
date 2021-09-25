#ifndef _GFX_RECT_IO_HPP
#define _GFX_RECT_IO_HPP
#include "gfx/rect.hpp"

#include <iosfwd>

namespace gfx
{
   template<typename T>
   inline std::ostream& operator<<(std::ostream& os, const RectT<T>& rc)
   {
      return os << "(" << rc.x << ", " 
                << rc.y << ", "
                << rc.w << ", "
                << rc.h << ")";
   }
} // namespace gfx

#endif