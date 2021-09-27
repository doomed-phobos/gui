#ifndef _SANDBOX_TIMER_HPP
#define _SANDBOX_TIMER_HPP
#include "base/debug.hpp"
#include <chrono>

namespace sandbox
{
   class AutoTimer
   {
   public:
      AutoTimer() {
         beg = std::chrono::system_clock::now();
      }

      ~AutoTimer() {
         end = std::chrono::system_clock::now();
         TRACE("Elapsed time: {}", std::chrono::duration<double>(end - beg).count());
      }
   private:
      std::chrono::system_clock::time_point beg, end;
   };
} // namespace sandbox


#endif