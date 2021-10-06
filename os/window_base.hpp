#ifndef _OS_WINDOW_BASE_HPP
#define _OS_WINDOW_BASE_HPP
#include "os/native_cursor.hpp"
#include "os/window_visible_mode.hpp"
#include "os/ref.hpp"
#include "gfx/fwd.hpp"
#include "base/signal.hpp"
#include "gfx/rect.hpp"

namespace os
{
   struct MouseEvent;
   struct KeyEvent;
   struct DropFilesEvent;
   struct ResizeEvent;
   class Surface;

   /// Clase abstracta usada para crear ventanas para cada plataforma, y, por ende, usar os::WindowBase para
   /// operaciones multiplataformas con ventanas 
   class WindowBase : public Referenceable
   {
   public:
      typedef void* NativeHandle;

      WindowBase();
      virtual ~WindowBase() {}

      virtual void setText(const char* text) = 0;
      virtual void setNativeCursor(NativeCursor cursor) = 0;
      void setBounds(int x, int y, int w, int h);
      void setBounds(const gfx::Rect& rc);
      void setPosition(const gfx::Point& pt);
      void setPosition(int x, int y);
      void setSize(const gfx::Size& sz);
      void setSize(int w, int h);
      /// Establece la escala de los graficos
      /// 
      /// @param scale Debe ser mayor a 0 para que surga efecto
      void setScale(int scale);
      /// Describes how to show the window and show it
      ///
      /// @see WindowVisibleMode
      virtual void setVisible(WindowVisibleMode mode) = 0;
      
      virtual void invalidate() = 0;
      virtual void captureMouse() = 0;
      virtual void releaseMouse() = 0;

      virtual bool isVisible() const = 0;
      virtual bool isMaximized() const = 0;
      int scale() const;
      virtual Surface& surface() const = 0;
      virtual NativeHandle handle() const = 0;
      /// Retorna el tamaño del cliente de la ventana
      virtual gfx::Point clientPointToScreenPoint(const gfx::Point& point) const = 0;
      virtual gfx::Point screenPointToClientPoint(const gfx::Point& point) const = 0;
      virtual gfx::Rect bounds() const = 0;
      /// Retorna la posición con respecto a la ventana (incluye bordes)
      gfx::Point position() const;
      /// Retorna el tamaño de la ventana
      gfx::Size size() const;

      // ==================
      // PUBLIC EVENTS
      // ==================
      base::Signal<void()> OnActivate;
      base::Signal<void()> OnDeactivate;
      base::Signal<void()> OnClose;
      base::Signal<void(const ResizeEvent&)> OnResize;
      base::Signal<void(const MouseEvent&)> OnMouseDown;
      base::Signal<void(const MouseEvent&)> OnMouseUp;
      base::Signal<void(const MouseEvent&)> OnMouseMove;
      base::Signal<void(const MouseEvent&)> OnMouseWheel;
      base::Signal<void(const MouseEvent&)> OnMouseDoubleClick;
      base::Signal<void(const MouseEvent&)> OnMouseLeave;
      base::Signal<void(/*const MouseEvent&*/)> OnMouseEnter;
      base::Signal<void(const KeyEvent&)> OnKeyDown;
      base::Signal<void(const KeyEvent&)> OnKeyUp;
      base::Signal<void(const DropFilesEvent&)> OnDropFiles;
   protected:
      /// ==================
      /// VIRTUAL EVENTS
      /// ==================
      /// Events signal their respective signals
      virtual void onActivate();
      virtual void onDeactivate();
      virtual void onClose();
      virtual void onResize(const ResizeEvent& ev);
      virtual void onMouseDown(const MouseEvent& ev);
      virtual void onMouseUp(const MouseEvent& ev);
      virtual void onMouseMove(const MouseEvent& ev);
      virtual void onMouseWheel(const MouseEvent& ev);
      virtual void onMouseDoubleClick(const MouseEvent& ev);
      virtual void onMouseLeave(const MouseEvent& ev);
      virtual void onMouseEnter();
      virtual void onKeyDown(const KeyEvent& ev);
      virtual void onKeyUp(const KeyEvent& ev);
      virtual void onDropFiles(const DropFilesEvent& ev);
      virtual void onPaint(Surface& surface);

      virtual void internalSetScale() = 0;
      virtual void internalSetBounds(int x, int y, int w, int h) = 0;
   private:
      int m_scale;
   };

   typedef SharedPtr<WindowBase> WindowPtr;
} // namespace os

#endif