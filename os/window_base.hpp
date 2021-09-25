#ifndef _OS_WINDOW_BASE_HPP
#define _OS_WINDOW_BASE_HPP
#include "os/native_cursor.hpp"
#include "os/ref.hpp"
#include "gfx/fwd.hpp"
#include "base/signal.hpp"

namespace os
{
   struct MouseEvent;
   struct KeyEvent;
   struct ChangedBoundsEvent;
   struct DropFilesEvent;
   struct ResizeEvent;
   class Surface;

   class WindowBase : public Referenceable
   {
   public:
      typedef void* NativeHandle;

      WindowBase();
      virtual ~WindowBase() {}

      virtual void setText(const char* text) = 0;
      virtual void setNativeCursor(NativeCursor cursor) = 0;
      virtual void setBounds(int x, int y, int w, int h) = 0;
      void setBounds(const gfx::Rect& rc);
      void setPosition(const gfx::Point& pt);
      void setPosition(int x, int y);
      void setSize(const gfx::Size& sz);
      void setSize(int w, int h);
      /// Establece la escala de los graficos
      /// 
      /// @param scale Debe ser mayor a 0 para que surga efecto
      void setScale(int scale);
      virtual void setVisible(bool state) = 0;
      
      virtual void invalidate() = 0;
      virtual void captureMouse() = 0;
      virtual void releaseMouse() = 0;

      virtual bool isVisible() const = 0;
      int scale() const;
      virtual Surface& surface() const = 0;
      virtual NativeHandle handle() const = 0;
      virtual gfx::Rect windowBounds() const = 0;
      virtual gfx::Rect clientBounds() const = 0;
      /// Retorna la posición con respecto a la ventana (incluye bordes)
      gfx::Point position() const;
      /// Retorna el tamaño del cliente de la ventana
      gfx::Size size() const;

      // ==================
      // PUBLIC EVENTS
      // ==================
      base::Signal<void()> OnActivate;
      base::Signal<void()> OnDeactivate;
      base::Signal<void(const ChangedBoundsEvent&)> OnChangedBound;
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
      // ==================
      // VIRTUAL EVENTS
      // ==================
      virtual void onActivate();
      virtual void onDeactivate();
      virtual void onChangedBound(const ChangedBoundsEvent& ev);
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

      virtual void onSetScale();
   private:
      int m_scale;
   };
} // namespace os

#endif