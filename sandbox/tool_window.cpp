#include "os/os.hpp"
#include "gfx/point.hpp"
#include "gfx/size.hpp"

#include <SkPaint.h>
#include <SkFont.h>

class ToolWindow : public os::Window
{
public:
   ToolWindow(os::Window& win) :
      os::Window(&win, "", (os::WindowStyle)(os::kFloating_WindowStyle | os::kPopup_WindowStyle)) {}
protected:
   virtual void onPaint(os::Surface& s) {
      // Simulate menu
      s.clear(SkColorSetRGB(32, 32, 32));
      SkPaint p;
      p.setAntiAlias(true);
      p.setStyle(SkPaint::kStroke_Style);
      p.setColor(SK_ColorDKGRAY);
      s.drawLine(SkPoint::Make(0.f, 0.f), gfx::Point(s.size()), p);
      s.drawLine(SkPoint::Make(s.width(), 0.f), SkPoint::Make(0.f, s.height()), p);
      p.setColor(SK_ColorWHITE);
      s.drawString("(Menu)", gfx::Point(s.size()/2.f), p, SkFont(), os::kCenter_TextAlign);
   }
};

class MainWindow : public os::Window
{
public:
   MainWindow(const char* text) :
      os::Window(text) {}
private:
   virtual void onPaint(os::Surface& s) {
      s.clear(SK_ColorRED);
   }
};

int main()
{
   os::SystemPtr sys(os::create_system());
   MainWindow win("Tool Window");
   ToolWindow tool(win);
   tool.setSize(500, 600);
   
   bool run = true;
   win.OnClose.connect([&]{run = false;});
   win.OnMouseDown.connect([&](const os::MouseEvent& ev) {
      if(!tool.isVisible()) {
         tool.setPosition(win.clientPointToScreenPoint(ev.position));
         tool.setVisible(os::WindowVisibleMode::kShow);
      } else {
         tool.setVisible(os::WindowVisibleMode::kHide);
      }
   });
   win.setVisible(os::WindowVisibleMode::kShowAndFocus);

   while(run) sys->waitEvents();

   return 0;
}