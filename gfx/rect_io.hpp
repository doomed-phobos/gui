#ifndef _GFX_RECT_IO_HPP
#define _GFX_RECT_IO_HPP
#include <iosfwd>
#include <fmt/format.h>

template<typename T>
struct fmt::formatter<gfx::RectT<T>> : public fmt::formatter<T>
{
   template<typename FormatContext>
   auto format(const gfx::RectT<T>& rc, FormatContext& ctx) const
      -> decltype(ctx.out()) {
      auto out = ctx.out();
      gfx::priv::write_text("(x: ", out);
      fmt::formatter<T>::format(rc.x, ctx);
      gfx::priv::write_text(", y: ", out);
      fmt::formatter<T>::format(rc.y, ctx);
      gfx::priv::write_text(", w: ", out);
      fmt::formatter<T>::format(rc.w, ctx);
      gfx::priv::write_text(", h: ", out);
      fmt::formatter<T>::format(rc.h, ctx);
      gfx::priv::write_text(")", out);
      return out;
   }
};

namespace gfx
{
   template<typename T>
   inline std::ostream& operator<<(std::ostream& os, const RectT<T>& rc)
   {
      return os << "(x: " << rc.x << ", y: " 
                << rc.y << ", w: "
                << rc.w << ", h: "
                << rc.h << ")";
   }
} // namespace gfx

#endif