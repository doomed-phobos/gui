#include "os/win/win_window.hpp"

#include "os/event.hpp"
#include "os/skia/skia_surface_paint.hpp"
#include "os/win/win_convert.hpp"
#include "os/win/win_keys.hpp"
#include "os/surface.hpp"
#include "base/exception.hpp"
#include "base/math.hpp"
#include "base/debug.hpp"
#include "base/log.hpp"

#include <iostream>
#include "gfx/size_io.hpp"

#include <algorithm>
#include <windowsx.h>
#include <commctrl.h>

namespace os::priv
{
   class WinWindow::WindowClass
   {
   public:
      static void Register() {
         WNDCLASSEX wcex{0};
         HINSTANCE hInstance = GetModuleHandle(nullptr);

         if(GetClassInfoEx(hInstance, strWndClass, &wcex))
            return; // Already registered

         wcex.cbSize = sizeof(WNDCLASSEX);
         wcex.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
         wcex.hInstance = hInstance;
         wcex.lpszClassName = strWndClass;
         wcex.lpfnWndProc = &WinWindow::ConfigureWndProc;
         wcex.style = CS_DBLCLKS;

         if(!RegisterClassEx(&wcex))
            throw base::Exception("Error al registrar WNDCLASS");
      }

      static constexpr const wchar_t* strWndClass = L"gui.window";
   };

   WinWindow::WinWindow(const char* text, WindowStyle style) :
      m_handle(nullptr),
      m_hasMouse(false),
      m_isCreated(false),
      m_surface(new Surface(0, 0))
   {
      WindowClass::Register();

      m_handle = CreateWindowEx(
         WS_EX_APPWINDOW | WS_EX_ACCEPTFILES,
         WindowClass::strWndClass,
         L"",
         WS_OVERLAPPEDWINDOW,
         CW_USEDEFAULT, CW_USEDEFAULT,
         CW_USEDEFAULT, CW_USEDEFAULT,
         nullptr,
         nullptr,
         GetModuleHandle(nullptr),
         reinterpret_cast<LPVOID>(this)
      );

      if(!m_handle)
         throw base::Exception("Error al crear ventana");
      
      SetWindowLongPtr(m_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
      m_isCreated = true;

      this->setText(text);
      this->setNativeCursor(NativeCursor::kArrow);
   }

   WinWindow::~WinWindow()
   {
      delete m_surface;
      if(m_handle)
         DestroyWindow(m_handle);
   }

   void WinWindow::setText(const char* text)
   {
      SetWindowText(m_handle, base::from_utf8(text).c_str());
   }

   void WinWindow::setNativeCursor(NativeCursor cursor)
   {
      HCURSOR hCursor = nullptr;

      switch(cursor) {
      case NativeCursor::kHidden:                  /*Do nothing*/                  break;
      case NativeCursor::kArrow:      hCursor = LoadCursor(nullptr, IDC_ARROW);    break;
      case NativeCursor::kIBeam:      hCursor = LoadCursor(nullptr, IDC_IBEAM);    break;
      case NativeCursor::kCrosshair:  hCursor = LoadCursor(nullptr, IDC_CROSS);    break;
      case NativeCursor::kHand:       hCursor = LoadCursor(nullptr, IDC_HAND);     break;
      case NativeCursor::kWait:       hCursor = LoadCursor(nullptr, IDC_WAIT);     break;
      case NativeCursor::kHelp:       hCursor = LoadCursor(nullptr, IDC_HELP);     break;
      case NativeCursor::kUpArrow:    hCursor = LoadCursor(nullptr, IDC_UPARROW);  break;
      case NativeCursor::kForbidden:  hCursor = LoadCursor(nullptr, IDC_NO);       break;
      case NativeCursor::kSizeNWSE:   hCursor = LoadCursor(nullptr, IDC_SIZENWSE); break;
      case NativeCursor::kSizeNESW:   hCursor = LoadCursor(nullptr, IDC_SIZENESW); break;
      case NativeCursor::kSizeWE:     hCursor = LoadCursor(nullptr, IDC_SIZEWE);   break;
      case NativeCursor::kSizeNS:     hCursor = LoadCursor(nullptr, IDC_SIZENS);   break;
      case NativeCursor::kSizeAll:    hCursor = LoadCursor(nullptr, IDC_SIZEALL);  break;
      }

      m_hCursor = hCursor;
   }

   void WinWindow::setBounds(int x, int y, int w, int h)
   {
      w = std::max(w - (w % this->scale()), 8*this->scale());
      h = std::max(h - (h % this->scale()), 8*this->scale());
      gfx::Size newSize = this->calculateWindowSizeFromClient(w, h);

      MoveWindow(m_handle, x, y,
         newSize.w,
         newSize.h, TRUE);
   }

   void WinWindow::onSetScale()
   {
      {
         RECT rc;
         GetWindowRect(m_handle, &rc);
         SendMessage(m_handle, WM_SIZING, 0, (LPARAM)&rc);
         SetWindowPos(m_handle, nullptr,
                        rc.left, rc.top,
                        rc.right - rc.left,
                        rc.bottom - rc.top,
                        SWP_NOZORDER | SWP_NOACTIVATE);
      }

      ResizeEvent ev;
      ev.newSize = m_clientSize;
      onResize(ev);
   }

   void WinWindow::setVisible(bool state)
   {
      if(state) {
         ShowWindow(m_handle, SW_SHOWDEFAULT);
         UpdateWindow(m_handle);
      } else {
         ShowWindow(m_handle, SW_HIDE);
      }
   }

   void WinWindow::invalidate()
   {
      InvalidateRect(m_handle, NULL, FALSE);
   }

   void WinWindow::captureMouse()
   {
      if(GetCapture() != m_handle)
         SetCapture(m_handle);
   }

   void WinWindow::releaseMouse()
   {
      if(GetCapture() == m_handle)
         ReleaseCapture();
   }

   bool WinWindow::isVisible() const
   {
      return IsWindowVisible(m_handle);
   }

   Surface& WinWindow::surface() const
   {
      return *m_surface;
   }

   WindowBase::NativeHandle WinWindow::handle() const
   {
      return reinterpret_cast<WindowBase::NativeHandle>(m_handle);
   }

   gfx::Rect WinWindow::windowBounds() const
   {
      RECT rc;
      GetWindowRect(m_handle, &rc);

      return from_win32(rc);
   }

   gfx::Rect WinWindow::clientBounds() const
   {
      RECT rc;
      GetClientRect(m_handle, &rc);

      return from_win32(rc);
   }

   LRESULT CALLBACK WinWindow::ConfigureWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
   {
      WinWindow* win = nullptr;

      if(msg == WM_CREATE) {
         win = reinterpret_cast<WinWindow*>(
               reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);

         if(win && win->m_handle == nullptr)
            win->m_handle = hWnd;
      } else {
         win = reinterpret_cast<WinWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

         if(win && win->m_handle != hWnd)
            win = nullptr;
      }

      if(win) {
         ASSERT(win->m_handle == hWnd);
         win->wndProc(msg, wParam, lParam);
      } else {
         return DefWindowProc(hWnd, msg, wParam, lParam);
      }
   }
   
   LRESULT WinWindow::wndProc(UINT msg, WPARAM wParam, LPARAM lParam)
   {
      switch(msg) {
      case WM_CREATE:
         LOG_INFO("Creating WinWindow {}", (void*)m_handle);
         break;
      case WM_DESTROY:
         LOG_INFO("Destroying WinWindow {}", (void*)m_handle);
         break;
      case WM_SETCURSOR:
         if(LOWORD(lParam) == HTCLIENT) {
            SetCursor(m_hCursor);
            return TRUE;
         }
         break;
      case WM_CLOSE:
         onClose();

         return FALSE;
         break;
      case WM_PAINT: {
         if(m_isCreated) {
            priv::SkiaSurfacePaint paint(m_handle, *m_surface, this->scale());

            onPaint(this->surface());
         }
         return TRUE;
      }
         break;
      case WM_SYSKEYDOWN:
      case WM_KEYDOWN: {
         int vk = wParam;
         int scancode = (lParam >> 16) & 0xff;
         bool sendMsg = true;
         const KeyCode keyCode = win32vk_to_keycode(vk);

         if((keyCode >= kKeyFirstModifierKeyCode) &&
            (lParam & (1 << 30)))
            return 0;
         
         KeyEvent ev;
         ev.modifiers = get_modifiers_from_last_win32_message();
         ev.keyCode = keyCode;
         ev.unicodeChar = 0;
         ev.repeat = std::max(0, int((lParam & 0xffff)-1));

         {
            VkToUnicode tu;
            if (tu) {
               tu.toUnicode(vk, scancode);
               if (tu.isDeadKey()) {
                  ev.isDeadKey = true;
                  ev.unicodeChar = tu[0];
                  // if (!m_translateDeadKeys)
                  //   tu.toUnicode(vk, scancode); // Call again to remove dead-key
               } else if (tu.size() > 0) {
                  sendMsg = false;
                  for (int chr : tu) {
                     ev.unicodeChar = chr;
                     onKeyDown(ev);
                  }
               }
            }
         }

         if (sendMsg)
         onKeyDown(ev);

         return 0;
      }
         break;
      case WM_SYSKEYUP:
      case WM_KEYUP: {
         KeyEvent ev;
         ev.modifiers = get_modifiers_from_last_win32_message();
         ev.keyCode = win32vk_to_keycode(wParam);
         ev.unicodeChar = 0;
         ev.repeat = std::max(0, int((lParam & 0xffff)-1));
         onKeyUp(ev);

         return 0;
      }
         break;
      case WM_MENUCHAR:
         return MAKELONG(0, MNC_CLOSE);
      case WM_SIZING: {
         LPRECT lpRect = reinterpret_cast<LPRECT>(lParam);

         int dx, dy;
         {
            RECT frame, client;
            GetWindowRect(m_handle, &frame);
            GetClientRect(m_handle, &client);
            dx = (frame.right - frame.left) - (client.right - client.left);
            dy = (frame.bottom - frame.top) - (client.bottom - client.top);
         }

         int w = std::max<int>(lpRect->right - lpRect->left, 0) - dx;
         int h = std::max<int>(lpRect->bottom - lpRect->top, 0) - dy;
         w = (w - (w % this->scale())) + dx;
         h = (h - (h % this->scale())) + dy;

         switch (wParam) {
         case WMSZ_LEFT:
            lpRect->left = lpRect->right - w;
            break;
         case WMSZ_RIGHT:
            lpRect->right = lpRect->left + w;
            break;
         case WMSZ_TOP:
            lpRect->top = lpRect->bottom - h;
            break;
         case WMSZ_TOPLEFT:
            lpRect->left = lpRect->right - w;
            lpRect->top = lpRect->bottom - h;
            break;
         case WMSZ_TOPRIGHT:
            lpRect->top = lpRect->bottom - h;
            lpRect->right = lpRect->left + w;
            break;
         case WMSZ_BOTTOM:
            lpRect->bottom = lpRect->top + h;
            break;
         case WMSZ_BOTTOMLEFT:
            lpRect->left = lpRect->right - w;
            lpRect->bottom = lpRect->top + h;
            break;
         case WMSZ_BOTTOMRIGHT:
            lpRect->right = lpRect->left + w;
            lpRect->bottom = lpRect->top + h;
            break;
         }
      }
         break;
      case WM_SIZE: {
         if(m_isCreated) {
            gfx::Size newSize = gfx::Size(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)) / this->scale();
            m_clientSize = newSize;

            ResizeEvent ev;
            ev.newSize = m_clientSize;
            m_surface->createRGBA(newSize.w, newSize.h);
            onResize(ev);
            this->invalidate();
         }
      }
         break;
      case WM_MOUSEMOVE: {
         MouseEvent ev;
         mouseEvent(lParam, ev);

         if(!m_hasMouse) {
            m_hasMouse = true;
            onMouseEnter();

            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(TRACKMOUSEEVENT);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = m_handle;
            _TrackMouseEvent(&tme);
         }

         onMouseMove(ev);
      }
         break;
      case WM_NCMOUSEMOVE:
      case WM_MOUSELEAVE:
         if(m_hasMouse) {
            m_hasMouse = false;

            MouseEvent ev;
            ev.modifiers = get_modifiers_from_last_win32_message();
            onMouseLeave(ev);
         }
         break;
      case WM_LBUTTONDOWN:
      case WM_MBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_XBUTTONDOWN: {
         MouseEvent ev;
         mouseEvent(lParam, ev);
         ev.button = (msg == WM_LBUTTONDOWN ? MouseEvent::kLeftButton :
                      msg == WM_MBUTTONDOWN ? MouseEvent::kMiddleButton :
                      msg == WM_RBUTTONDOWN ? MouseEvent::kRightButton : MouseEvent::kUnknownButton);
         onMouseDown(ev);
      }
         break;
      case WM_LBUTTONUP:
      case WM_MBUTTONUP:
      case WM_RBUTTONUP:
      case WM_XBUTTONUP: {
         MouseEvent ev;
         mouseEvent(lParam, ev);
         ev.button = (msg == WM_LBUTTONUP ? MouseEvent::kLeftButton :
                      msg == WM_MBUTTONUP ? MouseEvent::kMiddleButton :
                      msg == WM_RBUTTONUP ? MouseEvent::kRightButton : MouseEvent::kUnknownButton);
         onMouseUp(ev);
      }
         break;
      case WM_LBUTTONDBLCLK:
      case WM_MBUTTONDBLCLK:
      case WM_RBUTTONDBLCLK:
      case WM_XBUTTONDBLCLK: {
         MouseEvent ev;
         mouseEvent(lParam, ev);
         ev.button = (msg == WM_LBUTTONDBLCLK ? MouseEvent::kLeftButton :
                      msg == WM_MBUTTONDBLCLK ? MouseEvent::kMiddleButton :
                      msg == WM_RBUTTONDBLCLK ? MouseEvent::kRightButton : MouseEvent::kUnknownButton);
         onMouseDoubleClick(ev);
      }
         break;
      case WM_MOUSEHWHEEL:
      case WM_MOUSEWHEEL: {
         POINT pos{GET_X_LPARAM(lParam),
                   GET_Y_LPARAM(lParam)};
         ScreenToClient(m_handle, &pos);

         MouseEvent ev;
         ev.position = gfx::Point(pos.x, pos.y) / this->scale();

         int z = GET_WHEEL_DELTA_WPARAM(wParam);
         if(base::abs(z) >= WHEEL_DELTA)
            z /= WHEEL_DELTA;
         else
            z = base::sign(z);

         ev.delta = gfx::Point((msg == WM_MOUSEHWHEEL ? z : 0),
                               (msg == WM_MOUSEWHEEL ? -z : 0));
         onMouseWheel(ev);  
      }
         break;
      case WM_HSCROLL:
      case WM_VSCROLL: {
         POINT pos;
         GetCursorPos(&pos);
         ScreenToClient(m_handle, &pos);

         MouseEvent ev;
         ev.position = gfx::Point(pos.x, pos.y) / this->scale();

         int bar = (msg == WM_HSCROLL ? SB_HORZ : SB_VERT);
         int z = GetScrollPos(m_handle, bar);

         switch(LOWORD(wParam)) {
         case SB_LEFT:
         case SB_LINELEFT:
            --z;
            break;
         case SB_PAGELEFT:
            z -= 2;
            break;
         case SB_RIGHT:
         case SB_LINERIGHT:
            ++z;
            break;
         case SB_PAGERIGHT:
            z += 2;
            break;
         case SB_THUMBPOSITION:
         case SB_THUMBTRACK:
         case SB_ENDSCROLL:
            /*Do nothing*/
            break;
         }

         gfx::Point delta(
            (msg == WM_HSCROLL ? (z-50) : 0),
            (msg == WM_VSCROLL ? (z-50) : 0)
         );

         ev.delta = delta;
         SetScrollPos(m_handle, bar, 50, FALSE);
         onMouseWheel(ev);
      }
         break;
      case WM_WINDOWPOSCHANGED: {
         LPWINDOWPOS lpWindowPos = reinterpret_cast<LPWINDOWPOS>(lParam);
         ChangedBoundsEvent ev;
         ev.newBounds.x = lpWindowPos->x;
         ev.newBounds.y = lpWindowPos->y;
         ev.newBounds.w = lpWindowPos->cx;
         ev.newBounds.h = lpWindowPos->cy;
         onChangedBound(ev);
      }
         break;
      case WM_SETFOCUS:
         onActivate();
         break;
      case WM_KILLFOCUS:
         onDeactivate();
         break;
      case WM_DROPFILES: {
         HDROP hDrop = reinterpret_cast<HDROP>(wParam);
         DropFilesEvent ev;
         base::paths& files = ev.files;
         
         int count = DragQueryFileW(hDrop, INFINITE, NULL, 0);
         for(int index = 0; index < count; ++index) {
            int required_size = DragQueryFileW(hDrop, index, NULL, 0);
            if(required_size > 0) {
               std::vector<wchar_t> buf(++required_size);
               DragQueryFileW(hDrop, index, &buf[0], buf.size());
               files.push_back(base::to_utf8(&buf[0], buf.size()));
            }
         }
         DragFinish(hDrop);

         onDropFiles(ev);
      }
         break;
      }

      return DefWindowProc(m_handle, msg, wParam, lParam);
   }

   gfx::Size WinWindow::calculateWindowSizeFromClient(int w, int h)
   {
      RECT rc{0, 0, w, h};
      AdjustWindowRectEx(&rc,
         GetWindowLong(m_handle, GWL_STYLE),
         FALSE,
         GetWindowLong(m_handle, GWL_EXSTYLE));
      return gfx::Size(rc.right-rc.left, rc.bottom-rc.top);
   }

   void WinWindow::mouseEvent(LPARAM lParam, MouseEvent& ev)
   {
      ev.modifiers = get_modifiers_from_last_win32_message();
      ev.position = gfx::Point(GET_X_LPARAM(lParam),
                               GET_Y_LPARAM(lParam)) / scale();
   }
} // namespace os::priv