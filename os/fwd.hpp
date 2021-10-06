#ifndef _OS_FWD_HPP
#define _OS_FWD_HPP

namespace base
{
   template<typename T>
   class SharedPtr;
}

namespace os
{
   template<typename T>
   using SharedPtr = base::SharedPtr<T>;

   class WindowBase;
   class Surface;
   class System;
   class FontManager;

   typedef SharedPtr<WindowBase> WindowPtr;
   typedef SharedPtr<System> SystemPtr;
   typedef SharedPtr<Surface> SurfacePtr;
} // namespace os

#endif