#ifndef _GFX_SIZE_IO_HPP
#define _GFX_SIZE_IO_HPP
#include "gfx/size.hpp"

#include <iosfwd>

namespace gfx
{
   template<typename T>
   std::ostream& operator<<(std::ostream& os, const SizeT<T>& sz)
   {
      return os << "(" << sz.w << ", "
                << sz.h << ")";
   }
} // namespace gfx

#endif