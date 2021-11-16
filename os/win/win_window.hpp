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
         WinWindow(WinWindow* parent, const char* text, WindowStyle style = kDefault_WindowStyle);
         WinWindow(const char* text, WindowStyle style = kDefault_WindowStyle);
         virtual ~WinWindow();

         virtual void setText(const char* text) override;
         virtual void setNativeCursor(NativeCursor cursor) override;
         virtual void setVisible(WindowVisibleMode mode) override;

         virtual void invalidate() override;
         virtual void captureMouse() override;
         virtual void releaseMouse() override;

         virtual bool isVisible() const override;
         virtual bool isMaximized() const override;
         virtual gfx::Point clientPointToScreenPoint(const gfx::Point& point) const override;
         virtual gfx::Point screenPointToClientPoint(const gfx::Point& point) const override;
         virtual Surface& surface() const override;
         virtual NativeHandle handle() const override;
         virtual gfx::Rect bounds() const override;
      protected:
         virtual void internal_setScale() override;
         virtual void internal_setBounds(int x, int y, int w, int h) override;
         virtual void onResize(const os::ResizeEvent& ev);
      private:
         class WindowClass;

         static LRESULT CALLBACK ConfigureWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
         LRESULT wndProc(UINT msg, WPARAM wParam, LPARAM lParam);
         void mouseEvent(LPARAM lParam, MouseEvent& ev);
         HWND createHWND(WinWindow* parent, WindowStyle style);
         gfx::Size getClampedSizeFromRECT(const RECT& rc);

         HWND m_handle;
         HCURSOR m_hCursor;
         bool m_shadow : 1;
         bool m_hasMouse : 1;
         bool m_isCreated : 1;
         Surface* m_surface;
      };
   } // namespace priv
} // namespace os

#endif