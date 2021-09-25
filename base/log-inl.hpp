#include "base/mutex.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_sinks-inl.h"

#include <iostream>

// Se ponde inline porque permite multiples definiciones
namespace base
{
   namespace priv
   {
      struct console_mutex
      {
         using mutex_t = Mutex;
         static mutex_t& mutex() {
            static mutex_t s_mutex;
            return s_mutex;
         }
      };
   } // namespace priv

   inline LogSystem::LogSystem()
   {
      spdlog::set_pattern("[%l %d/%m/%Y %r]: %v");
      spdlog::set_default_logger(
         spdlog::create_async<spdlog::sinks::stdout_sink<priv::console_mutex>>("gui_log"));
   }

   inline LogSystem::~LogSystem()
   {
      spdlog::shutdown();
   }

   inline void LogSystem::log(Level level, const char* message)
   {
      log(level, "{}", message);
   }

   template<typename... Args>
   inline void LogSystem::log(Level level, const char* format, Args&&... args)
   {
      spdlog::log((spdlog::level::level_enum)level, format, std::forward<Args>(args)...);
   }

   inline LogSystem* LogSystem::GetInstance()
   {
      static LogSystem instance;
      return &instance;
   }
} // namespace base

#define LOG_INFO(...) base::LogSystem::GetInstance()->log(base::LogSystem::kInfo_Level, __VA_ARGS__)
#define LOG_WARNING(...) base::LogSystem::GetInstance()->log(base::LogSystem::kWarning_Level, __VA_ARGS__)
#define LOG_ERROR(...) base::LogSystem::GetInstance()->log(base::LogSystem::kError_Level, __VA_ARGS__)
#define LOG_CRITICAL(...) base::LogSystem::GetInstance()->log(base::LogSystem::kCritical_Level, __VA_ARGS__)