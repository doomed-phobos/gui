#ifndef _BASE_DEBUG_HPP
#define _BASE_DEBUG_HPP

#ifdef CORE_DEBUG
   #ifdef CORE_WINDOWS
   #  include <crtdbg.h>
   #  define base_break() _CrtDbgBreak()
   #else
   #  include <signal.h>
   #  define base_break() raise(SIGTRAP)
   #endif
   
   namespace base::priv
   {
      bool base_assert(const char* condition, const char* file, int lineNum);
      template<typename... Args>
      void base_trace(const char* format, Args&&... args);
   } // namespace base::priv
   
   #include "base/debug-inl.hpp"

   #define ASSERT(cond) do {                                    \
      if(!(cond))                                               \
         if(base::priv::base_assert(#cond, __FILE__, __LINE__)) \
            base_break();                                       \
   } while(0)

   #define TRACE base::priv::base_trace
#else
   #define ASSERT(cond)
   #define TRACE(...)
#endif

#endif