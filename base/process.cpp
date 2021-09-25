#include "base/process.hpp"

#ifdef CORE_WINDOWS
#  include <windows.h>
#else
#  include <signal.h>
#  include <sys/types.h>
#  include <unistd.h>
#endif

namespace base
{
   pid_t get_current_process_id()
   {
      #ifdef CORE_WINDOWS
      return (pid_t)GetCurrentProcessId();
      #else
      return (pid_t)getpid();
      #endif
   }
} // namespace base