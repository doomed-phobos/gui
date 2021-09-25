#include "base/string.hpp"

#include <iostream>
#include <vector>
#ifdef CORE_WINDOWS
#  include <windows.h>
#endif

namespace base::priv
{
   inline bool base_assert(const char* condition, const char* file, int lineNum)
   {
      #ifdef CORE_WINDOWS
      std::vector<wchar_t> buf(MAX_PATH);
      GetModuleFileNameW(nullptr, &buf[0], (DWORD)buf.size());

      int ret = _CrtDbgReportW(
         _CRT_ASSERT,
         from_utf8(file).c_str(),
         lineNum,
         &buf[0],
         from_utf8(condition).c_str()
      );

      return ret == 1 ? true : false;
      #else
      std::cerr << format_to_string("Assertion failed in %s %d: %s", file, lineNum, condition) << std::endl;
      std::abort();
      return true;
      #endif
   }

   template<typename... Args>
   inline void base_trace(const char* format, Args&&... args)
   {
      std::cerr << format_to_string(format, std::forward<Args>(args)...) << std::endl;
   }
} // namespace base::priv