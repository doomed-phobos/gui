#include "os/os.hpp"
#include "gfx/size.hpp"
#include "gfx/point.hpp"
#include "gfx/size_io.hpp"
#include "base/log.hpp"

os::SystemPtr sys;

void centerWindow(os::WindowBase* window)
{
   gfx::Size workArea = sys->getWorkareaSizeFromWindow(window);
   window->setPosition(gfx::Point(workArea/2 - window->size()/2));
}

int main()
{
   try {
      sys = os::create_system();
      bool run = true;
      os::Window win("Simple Window");
      win.OnClose.connect([&]{run = false;});
      win.setSize(800, 400);
      centerWindow(&win);
      win.setVisible(os::WindowVisibleMode::kShowAndFocus);
   
      while(run) sys->waitEvents();
   } catch(const std::exception& e) {
      MessageBox(nullptr, base::from_utf8(e.what()).c_str(), L"Critical Error", MB_ABORTRETRYIGNORE);
   }

   return 0;
}