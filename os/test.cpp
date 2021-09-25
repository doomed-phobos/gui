#include "os/system.hpp"
#include "os/window.hpp"
#include "os/surface.hpp"

#include "SkFont.h"
#include "SkPaint.h"

#include <iostream>
#include <windows.h>
using namespace std;

class MainWindow : public os::Window
{
public:
   MainWindow() :
      os::Window("Main Window") {
      this->setSize(800, 400);
      this->setVisible(true);
      m_run = true;
   }

   bool isRun() const {
      return m_run;
   }
protected:
   virtual void onPaint(os::Surface& s) {
      s.clear(SK_ColorTRANSPARENT);

      SkPaint p;
      p.setColor(SK_ColorRED);
      p.setStyle(SkPaint::kFill_Style);
      s.drawString("HOLA MUNDO", 100.f, 100.f, p, SkFont());
   }

   virtual void onClose() {
      m_run = false;
   }
private:
   bool m_run;
};

int main()
{
   try {
      os::SystemPtr sys(os::create_system());
      MainWindow win;
      
      while(win.isRun())
         sys->waitEvents();
   } catch(const exception& e) {
      MessageBox(nullptr, base::from_utf8(e.what()).c_str(), TEXT("Exception"), MB_ABORTRETRYIGNORE);
   }

   return 0;
}