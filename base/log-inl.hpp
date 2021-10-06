#include "base/scoped_lock.hpp"
#include "base/string.hpp"

#include <iostream>
#include <fmt/chrono.h>

// Se pone inline porque permite multiples definiciones
namespace base
{
   namespace priv
   {
      static constexpr inline const char* loglevel_to_cstr(LogSystem::Level level)
      {
         switch(level) {
            case LogSystem::kInfo_Level: return "Info";
            case LogSystem::kError_Level: return "Error";
            case LogSystem::kCritical_Level: return "Critical";
            case LogSystem::kWarning_Level: return "Warning";
            default: return "Unknown Level";
         }
      }
   } // namespace priv

   inline void LogSystem::log(Level level, const char* message)
   {
      log(level, "{}", message);
   }

   template<typename... Args>
   inline void LogSystem::log(Level level, const char* format, Args&&... args)
   {
      static Mutex s_mutex;
      ScopedLock lock(s_mutex);
      std::cerr << format_to_string("[{} {:%d/%m/%Y %r}]: {}",
         priv::loglevel_to_cstr(level),
         fmt::localtime(std::time(nullptr)),
         format_to_string(format, std::forward<Args>(args)...)) << std::endl;
   }
} // namespace base

#define LOG_INFO(...) base::LogSystem::log(base::LogSystem::kInfo_Level, __VA_ARGS__)
#define LOG_WARNING(...) base::LogSystem::log(base::LogSystem::kWarning_Level, __VA_ARGS__)
#define LOG_ERROR(...) base::LogSystem::log(base::LogSystem::kError_Level, __VA_ARGS__)
#define LOG_CRITICAL(...) base::LogSystem::log(base::LogSystem::kCritical_Level, __VA_ARGS__)