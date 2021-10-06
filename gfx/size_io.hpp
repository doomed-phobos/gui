#ifndef _GFX_SIZE_IO_HPP
#define _GFX_SIZE_IO_HPP
#include "gfx/gfx_io.hpp"

#include <iosfwd>

template<typename T>
struct fmt::formatter<gfx::SizeT<T>> : public fmt::formatter<T>
{
   template<typename FormatContext>
   auto format(const gfx::SizeT<T>& sz, FormatContext& ctx) const
      -> decltype(ctx.out()) {
      auto out = ctx.out();
      gfx::priv::write_text("(w: ", out);
      fmt::formatter<T>::format(sz.w, ctx);
      gfx::priv::write_text(", h: ", out);
      fmt::formatter<T>::format(sz.h, ctx);
      gfx::priv::write_text(")", out);
      return out;
   }
};

namespace gfx
{
   template<typename T>
   std::ostream& operator<<(std::ostream& os, const SizeT<T>& sz)
   {
      return os << "(w: " << sz.w << ", h: "
                << sz.h << ")";
   }
} // namespace gfx

#endif