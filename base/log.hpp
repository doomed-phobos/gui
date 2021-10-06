#ifndef _BASE_LOG_HPP
#define _BASE_LOG_HPP
#include "base/config.hpp"

namespace base
{
   class LogSystem
   {
   public:
      enum Level {
         kInfo_Level,
         kWarning_Level,
         kError_Level,
         kCritical_Level
      };

      static void log(Level level, const char* message);
      template<typename... Args>
      static void log(Level level, const char* format, Args&&... args);
   };
} // namespace base

#include "base/log-inl.hpp"

/// ================
/// MACROS
/// ================
/// LOG_INFO(...)       => Information about something
/// LOG_WARNING(...)    => Something failed, the UI don't need to show this, and we can continue
/// LOG_ERROR(...)      => Something failed, the UI should show this, and we can continue
/// LOG_CRITICAL(...)   => Something failed and we CANNOT continue the execution, used in exceptions

#endif