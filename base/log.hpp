#ifndef _BASE_LOG_HPP
#define _BASE_LOG_HPP

namespace base
{
   class LogSystem
   {
   public:
      enum Level {
         kInfo_Level     = 2,
         kWarning_Level  = 3,
         kError_Level    = 4,
         kCritical_Level = 5
      };

      ~LogSystem();
      
      void log(Level level, const char* message);
      template<typename... Args>
      void log(Level level, const char* format, Args&&... args);

      static LogSystem* GetInstance();
   private:
      LogSystem();
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