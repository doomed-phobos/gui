#include "os/os.hpp"
#include "gfx/point.hpp"
#include "gfx/point_io.hpp"
#include "gfx/rect.hpp"

#include <SkPaint.h>
#include <SkFont.h>

class PanWindow : public os::Window
{
public:
   PanWindow(const char* text) :
      os::Window(text),
      m_zoom(1.f),
      m_hasCapture(false) {
      setVisible(os::WindowVisibleMode::kShowAndFocus);
      m_running = true;
   }
   
   bool isRunning() const {return m_running;}
private:
   typedef os::Window INHERITED;
protected:
   void onClose() override {
      m_running = false;

      INHERITED::onClose();
   }
   void onMouseDown(const os::MouseEvent& ev) override {
      if(!m_hasCapture) {
         m_hasCapture = true;
         this->setNativeCursor(os::NativeCursor::kHand);
         this->captureMouse();
         m_capturePos = ev.position;
         m_captureScroll = m_scroll;
      }

      INHERITED::onMouseDown(ev);
   }

   void onMouseUp(const os::MouseEvent& ev) override {
      if(m_hasCapture) {
         m_hasCapture = false;
         this->setNativeCursor(os::NativeCursor::kDefault);
         this->releaseMouse();
      }

      INHERITED::onMouseUp(ev);
   }
   
   void onMouseMove(const os::MouseEvent& ev) override {
      if(m_hasCapture) {
         m_scroll = m_captureScroll + gfx::PointF(ev.position - m_capturePos);
         this->invalidate();
      }

      INHERITED::onMouseMove(ev);
   }

   void onMouseWheel(const os::MouseEvent& ev) override {
      if(ev.isPressingKeyModifiers(os::kKeyCtrlModifier)) {
         int z = (ev.delta.x + ev.delta.y);
         setZoom(gfx::PointF(ev.position), m_zoom - z/10.0);
         this->invalidate();
      }

      INHERITED::onMouseWheel(ev);
   }

   void onPaint(os::Surface& s) override {
      s.clear(SkColorSetRGB(32, 32, 32));
      
      SkPaint p;
      p.setStyle(SkPaint::kStroke_Style);
      p.setColor(SkColorSetRGB(255, 255, 200));
      {
         gfx::RectF rc(s.bounds());
         rc.shrink(24.f);
         rc.offset(-center());
         rc *= m_zoom;
         rc.offset(center());
         rc.offset(m_scroll);
         s.drawRect(rc, p);

         for(int i = 1; i < 8; ++i) {
            int v = i * rc.w / 8;
            s.drawLine(rc.x + v, rc.y,
                       rc.x + v, rc.y + rc.h, p);
            v = i * rc.h / 8;
            s.drawLine(rc.x,        rc.y + v,
                       rc.x + rc.w, rc.y + v, p);
         }
      }

      {
         p.setStyle(SkPaint::kFill_Style);
         s.drawString(base::format_to_string("Scroll={:.2f}  Zoom={:.2f}", m_scroll,  m_zoom), {12.f, 12.f}, p, SkFont());
      }
      
      INHERITED::onPaint(s);
   }
private:
   void setZoom(const gfx::PointF& mousePos, double newZoom) {
      double oldZoom = m_zoom;
      m_zoom = base::clamp(newZoom, 0.01, 10.0);
      
      m_scroll = mousePos - (mousePos - m_scroll - center()) * m_zoom / oldZoom - center();
   }

   gfx::PointF center() const {
      return gfx::PointF(size()/2.f);
   }

   bool m_running;
   gfx::PointF m_scroll;
   float m_zoom;

   bool m_hasCapture;
   gfx::Point m_capturePos;
   gfx::PointF m_captureScroll;
};

int main()
{
   os::SystemPtr sys(os::create_system());
   PanWindow win("Pan Viewport");

   while(win.isRunning())
      sys->waitEvents();

   return 0;
}