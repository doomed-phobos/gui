#include "fmt/xchar.h"

namespace base
{
   namespace priv
   {
      template<typename Char, typename... Args>
      static inline
         std::basic_string<Char>
         format_to_tstring(const Char* format, Args&&... args)
      {
         return fmt::format(format, std::forward<Args>(args)...);
      }
   }

   template<typename... Args>
   inline std::string format_to_string(const char* format, Args&&... args)
   {
      return priv::format_to_tstring(format, std::forward<Args>(args)...);
   }

   template<typename... Args>
   inline std::wstring format_to_string(const wchar_t* format, Args&&... args)
   {
      return priv::format_to_tstring(format, std::forward<Args>(args)...);
   }
} // namespace base