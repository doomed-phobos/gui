#include "os/os.hpp"
#include "gfx/point.hpp"
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
protected:
   virtual void onClose() {m_running = false;}
   virtual void onMouseMove(const os::MouseEvent& ev) {
      
   }
   virtual void onPaint(os::Surface& s) {
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
         // s.drawString(base::format_to_string("Scroll={:.2f}  Zoom={}"), {12.f, 12.f}, p, SkFont());
      }
   }
private:
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