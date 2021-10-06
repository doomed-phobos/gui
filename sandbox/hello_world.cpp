#include "os/os.hpp"
#include "SkPaint.h"
#include "SkFont.h"
#include "gfx/rect_io.hpp"
#include "gfx/size_io.hpp"
#include "gfx/point_io.hpp"

class MainWindow : public os::Window
{
public:
   MainWindow(const char* text) : 
      os::Window(text) {}
private:
   virtual void onPaint(os::Surface& s) {
      s.clear(SK_ColorBLACK);

      gfx::SizeF sz(s.size());
      SkPaint p;
      p.setStyle(SkPaint::kFill_Style);
      p.setColor(SkColorSetRGB(255, 0, 0));     s.drawLine(0.f, 0.f, sz.w, sz.h, p);
      p.setColor(SkColorSetRGB(0, 128, 0));     s.drawLine(sz.w/2.f, 0.f, sz.w/2.f, sz.h, p);
      p.setColor(SkColorSetRGB(255, 255, 255)); s.drawLine(sz.w, 0.f, 0, sz.h, p);
      p.setColor(SK_ColorWHITE);

      SkPoint center{sz.w/2.f, sz.h/2.f};
      s.drawString("Hello World", center, p, SkFont(), os::kCenter_TextAlign);
      p.setColor(SK_ColorBLUE);
      s.drawCircle(center, 10.f, p);
   }
};

int main()
{
   os::SystemPtr sys(os::create_system());
   MainWindow win("Hello World");
   win.setSize(800, 400);

   bool running = true;
   win.OnKeyDown.connect([&](const os::KeyEvent& ev) {
      switch(ev.keyCode) {
      case os::kKeyEsc:
         running = false;
         break;
      case os::kKey1:
      case os::kKey2:
      case os::kKey3:
      case os::kKey4:
      case os::kKey5:
      case os::kKey6:
      case os::kKey7:
      case os::kKey8:
      case os::kKey9:
         win.setScale(1 + (int)(ev.keyCode - os::kKey1));
         break;
      }
      win.invalidate();
   });
   win.OnClose.connect([&] {running = false;});
   win.setVisible(os::WindowVisibleMode::kShowAndFocus);

   while(running)
      sys->waitEvents();

   return 0;
}