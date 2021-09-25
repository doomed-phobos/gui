#include "base/mutex.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_sinks-inl.h>

// Se pone inline porque permite multiples definiciones
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
      spdlog::create<spdlog::sinks::stderr_sink<priv::console_mutex>>("gui_log");
      spdlog::get("gui_log")->set_pattern("[%l %d/%m/%Y %r]: %v");
      spdlog::get("gui_log")->set_error_handler([this](const std::string& msg) {
         this->log(kError_Level, "(spdlog) {}", msg);
      });
   }

   inline LogSystem::~LogSystem()
   {
   }

   inline void LogSystem::log(Level level, const char* message)
   {
      log(level, "{}", message);
   }

   template<typename... Args>
   inline void LogSystem::log(Level level, const char* format, Args&&... args)
   {
      spdlog::get("gui_log")->log((spdlog::level::level_enum)level, format, std::forward<Args>(args)...);
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