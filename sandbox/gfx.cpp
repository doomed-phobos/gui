#include "gfx/point_io.hpp"
#include "base/debug.hpp"
#include <fmt/format.h>

// TODO: Add gfx::Point to fmt
template<typename T>
struct fmt::formatter<gfx::PointT<T>>
{
   auto format(const gfx::PointT<T>& pt, fmt::format_context ctx) const
      -> decltype(ctx.out()) {
      fmt::formatter<int>::format()
   }
};

int main()
{
   TRACE("pt: {}", gfx::Point(1, 1));

   return 0;
}