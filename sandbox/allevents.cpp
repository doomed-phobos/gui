#include "os/os.hpp"
#include "gfx/gfx.hpp"

#include <vector>

class LogWindow : public os::Window
{
public:
   LogWindow() :
      os::Window("All Events") {
      
      recalcMaxLines();

      logLine("-- Events Log --");

      this->OnActivate.connect([this] {
         logLine("OnActivate Window");
      });
      this->OnDeactivate.connect([this] {
         logLine("OnDesactivate Window");
      });
      this->OnMouseDown.connect([this](const os::MouseEvent& ev) {this->onMouseEvent(ev, "OnMouseDown");});
      this->OnMouseUp.connect([this](const os::MouseEvent& ev) {this->onMouseEvent(ev, "OnMouseUp");});
      this->OnMouseMove.connect([this](const os::MouseEvent& ev) {this->onMouseEvent(ev, "OnMouseMove");});
      this->OnMouseEnter.connect([this]() {this->onMouseEvent(os::MouseEvent(), "OnMouseEnter");});
      this->OnMouseLeave.connect([this](const os::MouseEvent& ev) {this->onMouseEvent(ev, "OnMouseLeave");});
      this->OnMouseDoubleClick.connect([this](const os::MouseEvent& ev) {this->onMouseEvent(ev, "OnMouseDoubleClick");});
      this->OnResize.connect([this](const os::ResizeEvent& ev) {
         logLine("Resize Window size={}", ev.newSize);
         recalcMaxLines();
      });
      this->OnDropFiles.connect([this](const os::DropFilesEvent& ev) {
         logLine("DropFiles files:{}", '{');
         for(const auto& file : ev.files)
            logLine("  \"{}\"", file);
         logLine("{}", '}');
      });
      this->OnMouseWheel.connect([this](const os::MouseEvent& ev) {
         m_mousePos = ev.position;
         logLine("MouseWheel pos={} wheel={}{}",
            ev.position,
            ev.delta,
            modifiersToString(ev.modifiers));
      });
      this->OnKeyDown.connect([this](const os::KeyEvent& ev) {
         onKeyEvent(ev, "OnKeyDown");
      });
      this->OnKeyUp.connect([this](const os::KeyEvent& ev) {
         onKeyEvent(ev, "OnKeyUp");
      });
      this->OnClose.connect([this] {m_running = false;});

      this->setSize(800, 600);
      this->setVisible(os::WindowVisibleMode::kShowAndFocus);
   }

   bool isRun() const {return m_running;}
private:
   typedef os::Window INHERITED;
   
   void onKeyEvent(const os::KeyEvent& ev, const char* eventName) {
      wchar_t wideUnicode[2] = {ev.unicodeChar, 0};
      logLine("{} keycode={} unicode={} ({}){}",
         eventName, ev.keyCode, ev.unicodeChar, base::to_utf8((const wchar_t*)&ev.unicodeChar, 1), modifiersToString(ev.modifiers));
   }

   void onMouseEvent(const os::MouseEvent& ev, const char* eventName) {
      logMouseEvent(ev, eventName);
   }
protected:
   void onPaint(os::Surface& s) override {
      if(m_oldLogSize != m_textLog.size()) {
         int newLines = m_textLog.size() - m_oldLogSize;
         while(m_textLog.size() > m_maxLines)
            m_textLog.erase(m_textLog.begin());
         
         internalPaint(s, newLines);

         m_oldLogSize = m_textLog.size();
      }

      INHERITED::onPaint(s);
   }
private:
   void internalPaint(os::Surface& s, const int newLines) {
      const SkRect rc = s.bounds();
      
      SkPaint p;
      p.setColor(SkColorSetARGB(15, 0, 0, 0));
      int i;
      if(m_textLog.size() >= m_maxLines) {
         int h = m_lineHeight*newLines;
         s.scrollTo(rc, 0, -h);

         s.drawRect(SkRect::MakeXYWH(rc.x(), rc.y(), rc.width(), rc.height()-h), p);
         p.setColor(SK_ColorBLACK);
         s.drawRect(SkRect::MakeXYWH(rc.x(), rc.y()+rc.height()-h, rc.width(), h), p);

         i = m_textLog.size() - newLines;
      } else {
         i = m_oldLogSize;
         s.drawRect(SkRect::MakeXYWH(rc.x(), rc.y(), rc.width(), i*(float)m_lineHeight), p);
      }

      p.setColor(SK_ColorWHITE);
      for(; i < m_textLog.size(); ++i)
         s.drawString(m_textLog[i], SkPoint::Make(0.f, (1.f+i)*m_lineHeight), p, SkFont());

      p.setAntiAlias(true);
      s.drawCircle(m_mousePos, m_brushSize, p);
   }

   void recalcMaxLines() {
      m_maxLines = (this->height() - m_lineHeight) / m_lineHeight;
   }

   template<typename... Args>
   void logLine(const char* format, Args&&... args) {
      m_textLog.push_back(base::format_to_string(format, std::forward<Args>(args)...));
      this->invalidate();
   }

   void logMouseEvent(const os::MouseEvent& ev, const char* eventName) {
      const os::MouseEvent::Button mb = ev.button;
      m_mousePos = ev.position;
      logLine("{} pos={}{}{}",
         eventName, m_mousePos,
         (mb == os::MouseEvent::kLeftButton ? " LeftButton" :
          mb == os::MouseEvent::kRightButton ? " RightButton" :
          mb == os::MouseEvent::kMiddleButton ? " MiddleButton" : ""), modifiersToString(ev.modifiers));
   }

   static std::string modifiersToString(os::KeyModifiers mods) {
      std::string s;
      if(mods & os::kKeyShiftModifier) s += " Shift";
      if(mods & os::kKeyCtrlModifier)  s += " Ctrl";
      if(mods & os::kKeyAltModifier)   s += " Alt";
      if(mods & os::kKeyCmdModifier)   s += " Command";
      if(mods & os::kKeySpaceModifier) s += " Space";
      if(mods & os::kKeyWinModifier)   s += " Win";
      return s;
   }

   bool m_running = true;
   size_t m_oldLogSize = 0;
   double m_brushSize = 4;
   gfx::Point m_mousePos;
   std::vector<std::string> m_textLog;
   int m_maxLines = 0;
   int m_lineHeight = 12;
};

int main()
{
   os::SystemPtr sys(os::create_system());
   LogWindow win;

   while(win.isRun()) {
      sys->waitEvents();
   }

   return 0;
}