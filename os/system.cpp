#include "os/system.hpp"

#include "os/surface.hpp"
#include "os/window.hpp"
#include "base/log.hpp"
#include "base/debug.hpp"
#include "base/files.hpp"
#include "gfx/size.hpp"

#include <SkCodec.h>
#include <memory>
#include <windows.h>

namespace os
{
   static System* g_system = nullptr;

   System::System() :
      m_defaultWindow(nullptr)
   {
      LOG_INFO("System creating successfully");
   }

   System::~System()
   {
      LOG_INFO("System destroying successfully");
      g_system = nullptr;
   }

   void System::waitEvents()
   {
      MSG msg;
      if(::GetMessage(&msg, nullptr, 0, 0) > 0){
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
   }

   void System::pollEvents()
   {
      MSG msg;
      if(::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0){
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }   
   }

   void System::setDefaultWindow(WindowBase* window)
   {
      m_defaultWindow = window;
   }

   WindowBase* System::defaultWindow() const
   {
      return m_defaultWindow;
   }

   gfx::Size System::getWorkareaSizeFromWindow(WindowBase* window) const
   {
      HMONITOR hMonitor = MonitorFromWindow(window ? (HWND)window->handle() : nullptr, MONITOR_DEFAULTTONEAREST);
      MONITORINFO mi;
      mi.cbSize = sizeof(MONITORINFO);
      GetMonitorInfo(hMonitor, &mi);
      return gfx::Size(mi.rcWork.right - mi.rcWork.left, mi.rcWork.bottom - mi.rcWork.top);
   }

   SurfacePtr System::createRGBASurface(int width, int height) const
   {
      return base::make_shared<Surface>(width, height);
   }

   SurfacePtr System::loadSurface(const char* filename) const
   {
      FILE* f = base::open_file_raw(filename, "rb");
      if (!f)
         return nullptr;

      std::unique_ptr<SkCodec> codec(
         SkCodec::MakeFromStream(
            std::unique_ptr<SkFILEStream>(new SkFILEStream(f))));
      if (!codec)
         return nullptr;

      SkImageInfo info = codec->getInfo()
         .makeColorType(kN32_SkColorType)
         .makeAlphaType(kPremul_SkAlphaType);
      SkBitmap bm;
      if (!bm.tryAllocPixels(info))
         return nullptr;

      const SkCodec::Result r = codec->getPixels(info, bm.getPixels(), bm.rowBytes());
      if (r != SkCodec::kSuccess)
         return nullptr;

      return base::make_shared<Surface>(bm);
   }

   System* System::GetInstance()
   {
      return g_system;
   }

   SystemPtr create_system()
   {
      ASSERT(!g_system);
      g_system = new System();
      return SystemPtr(g_system);
   }
} // namespace os