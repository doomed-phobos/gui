#ifndef _GFX_POINT_IO_HPP
#define _GFX_POINT_IO_HPP
#include "gfx/gfx_io.hpp"

#include <iosfwd>

template<typename T>
struct fmt::formatter<gfx::PointT<T>> : public fmt::formatter<T>
{
   template<typename FormatContext>
   auto format(const gfx::PointT<T>& pt, FormatContext& ctx) const
      -> decltype(ctx.out()) {
      auto out = ctx.out();
      gfx::priv::write_text("(x: ", out);
      fmt::formatter<T>::format(pt.x, ctx);
      gfx::priv::write_text(", y: ", out);
      fmt::formatter<T>::format(pt.y, ctx);
      gfx::priv::write_text(")", out);
      return out;
   }
};

namespace gfx
{
   template<typename T>
   inline std::ostream& operator<<(std::ostream& os, const PointT<T>& point)
   {
      return os << "(x: "
               << point.x << ", y: "
               << point.y << ")";
   }
} // namespace gfx

#endif