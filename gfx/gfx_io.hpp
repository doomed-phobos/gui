#ifndef _GFX_IO_HPP
#define _GFX_IO_HPP
#include <fmt/format.h>

namespace gfx::priv
{
   template<typename Char, typename OutputIt>
   static OutputIt write_text(const Char* str, OutputIt out)
   {
      auto str_view = fmt::to_string_view(str);
      return std::copy(str_view.begin(), str_view.end(), out);
   }
} // namespace gfx::priv

#endif