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
   void onPaint(os::Surface& s) override {
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
    
      os::Window::onPaint(s);
   }
};

class MainWindow : public os::Window
{
public:
   MainWindow(const char* text) :
      os::Window(text) {}
private:
   void onPaint(os::Surface& s) override {
      s.clear(SK_ColorRED);

      os::Window::onPaint(s);
   }
};

int main()
{
   os::SystemPtr sys(os::create_system());
   MainWindow win("Tool Window");
   ToolWindow tool(win);
   tool.setSize(100, 120);
   
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