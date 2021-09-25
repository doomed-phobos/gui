#include "base/exception.hpp"

#include "base/log.hpp"

#ifdef CORE_WINDOWS
#include <windows.h>
#endif

namespace base
{
   Exception::Exception() :
      std::exception()
   {
      init(std::string());
   } 

   Exception::Exception(const std::string& msg) :
      std::exception()
   {
      init(msg);
   }

   Exception::Exception(const char* format, ...) :
      std::exception()
   {
      std::va_list ap;
      va_start(ap, format);
      std::string res = format_to_string(format, ap);
      va_end(ap);

      init(res);
   }

   char const* Exception::what() const noexcept
   {
      return m_msg.c_str();
   }

   void Exception::init(const std::string& msg)
   {
      #ifdef CORE_WINDOWS
      HMODULE hModule = nullptr;
      DWORD dwFlags =
         FORMAT_MESSAGE_ALLOCATE_BUFFER |
         FORMAT_MESSAGE_FROM_SYSTEM |
         FORMAT_MESSAGE_IGNORE_INSERTS;
      LPWSTR strMsgbuf = NULL;
      DWORD dwLastError = GetLastError();

      if(!FormatMessageW(dwFlags,
                  hModule,
                  dwLastError,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  reinterpret_cast<LPWSTR>(&strMsgbuf),
                  0, NULL))
         strMsgbuf = NULL;

      if(strMsgbuf) {
         std::wstring dbgmsg = format_to_string(L"Last Error: %d - %s", dwLastError, strMsgbuf);
         OutputDebugStringW(dbgmsg.c_str());
         LocalFree(strMsgbuf);
      }
      #endif
      m_msg = msg;
      LOG_CRITICAL(m_msg.c_str());
   }
} // namespace base