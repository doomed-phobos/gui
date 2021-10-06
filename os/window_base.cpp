#include "os/window_base.hpp"

#include "gfx/rect.hpp"
#include "gfx/point.hpp"
#include "gfx/size.hpp"

namespace os
{
   WindowBase::WindowBase() :
      m_scale(1)
   {}

   void WindowBase::setBounds(int x, int y, int w, int h)
   {
      this->internalSetBounds(x, y, w*this->scale(), h*this->scale());
   }

   void WindowBase::setBounds(const gfx::Rect& rc)
   {
      setBounds(rc.x, rc.y, rc.w, rc.h);
   }

   void WindowBase::setPosition(const gfx::Point& pt)
   {
      setBounds(gfx::Rect(pt, size()));
   }

   void WindowBase::setPosition(int x, int y)
   {
      setPosition(gfx::Point(x, y));
   }

   void WindowBase::setSize(const gfx::Size& sz)
   {
      setBounds(gfx::Rect(position(), sz));
   }

   void WindowBase::setSize(int w, int h)
   {
      setSize(gfx::Size(w, h));
   }
   
   void WindowBase::setScale(int scale)
   {
      if(scale < 0)
         return;
      
      m_scale = scale;

      this->internalSetScale();
   }

   int WindowBase::scale() const
   {
      return m_scale;
   }

   gfx::Point WindowBase::position() const
   {
      return bounds().origin();
   }

   gfx::Size WindowBase::size() const
   {
      return bounds().size();
   }

   void WindowBase::onActivate()
   {
      this->OnActivate();
   }

   void WindowBase::onDeactivate()
   {
      this->OnDeactivate();
   }

   void WindowBase::onClose()
   {
      this->OnClose();
   }

   void WindowBase::onResize(const ResizeEvent& ev)
   {
      this->OnResize(ev);
   }

   void WindowBase::onMouseDown(const MouseEvent& ev)
   {
      this->OnMouseDown(ev);
   }

   void WindowBase::onMouseUp(const MouseEvent& ev)
   {
      this->OnMouseUp(ev);
   }

   void WindowBase::onMouseMove(const MouseEvent& ev)
   {
      this->OnMouseMove(ev);
   }

   void WindowBase::onMouseWheel(const MouseEvent& ev)
   {
      this->OnMouseWheel(ev);
   }

   void WindowBase::onMouseDoubleClick(const MouseEvent& ev)
   {
      this->OnMouseDoubleClick(ev);
   }

   void WindowBase::onMouseLeave(const MouseEvent& ev)
   {
      this->OnMouseLeave(ev);
   }

   void WindowBase::onMouseEnter()
   {
      this->OnMouseEnter();
   }

   void WindowBase::onKeyDown(const KeyEvent& ev)
   {
      this->OnKeyDown(ev);
   }

   void WindowBase::onKeyUp(const KeyEvent& ev)
   {
      this->OnKeyUp(ev);
   }

   void WindowBase::onDropFiles(const DropFilesEvent& ev)
   {
      this->OnDropFiles(ev);
   }

   void WindowBase::onPaint(Surface& ev)
   {
      // Do nothing
   }
} // namespace os