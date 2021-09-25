#ifndef _OS_WIN_WINDOW_HPP
#define _OS_WIN_WINDOW_HPP
#include "os/window_base.hpp"
#include "os/window_style.hpp"
#include "gfx/size.hpp"

#include <windows.h>

namespace os
{
   namespace priv
   {
      class WinWindow : public WindowBase
      {
      public:
         WinWindow(const char* text, WindowStyle style = kDefault_WindowStyle);
         virtual ~WinWindow();

         virtual void setText(const char* text) override;
         virtual void setNativeCursor(NativeCursor cursor) override;
         virtual void setBounds(int x, int y, int w, int h) override;
         virtual void setVisible(bool state) override;

         virtual void invalidate() override;
         virtual void captureMouse() override;
         virtual void releaseMouse() override;

         virtual bool isVisible() const override;
         virtual Surface& surface() const override;
         virtual NativeHandle handle() const override;
         virtual gfx::Rect windowBounds() const override;
         virtual gfx::Rect clientBounds() const override;
      private:
         virtual void onSetScale();
      private:
         class WindowClass;

         static LRESULT CALLBACK ConfigureWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
         LRESULT wndProc(UINT msg, WPARAM wParam, LPARAM lParam);
         void mouseEvent(LPARAM lParam, MouseEvent& ev);
         gfx::Size calculateWindowSizeFromClient(int w, int h);

         HWND m_handle;
         HCURSOR m_hCursor;
         gfx::Size m_clientSize;
         bool m_hasMouse : 1;
         bool m_isCreated : 1;
         Surface* m_surface;
      };
   } // namespace priv
} // namespace os

#endif