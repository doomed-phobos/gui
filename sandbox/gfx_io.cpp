#include "timer.hpp"
#include "gfx/point.hpp"
#include "gfx/point_io.hpp"
#include "gfx/size.hpp"
#include "gfx/size_io.hpp"
#include "gfx/rect.hpp"
#include "gfx/rect_io.hpp"

#include <iostream>

#define FORMAT(...) {        \
   sandbox::AutoTimer timer; \
   TRACE(__VA_ARGS__);       \
}

#define COUT(...) {                       \
   sandbox::AutoTimer timer;              \
   std::cout << __VA_ARGS__ << std::endl; \
}

int main()
{
   gfx::Point p(255, 128);
   FORMAT("pt format: {:#X}\n", p);
   COUT("pt cout: " << p);
   gfx::SizeF s(100.645f, 200.4324f);
   FORMAT("sz format: {}\n", s);
   COUT("sz cout: " << s);
   gfx::RectF r(10.f, 10.f, 800.54f, 400.435f);
   FORMAT("rc format: {}\n", r);
   COUT("rc cout: " << r);

   return 0;
}