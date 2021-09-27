#include "base/thread.hpp"

#ifdef CORE_WINDOWS
#  include <windows.h>
#else
#  include <pthread.h>
#  include <unistd.h>
#  include <sys/time.h>
#endif

namespace base
{
   #ifdef CORE_WINDOWS
   static DWORD WINAPI win32_thread_proxy(LPVOID data)
   {
      Thread::details::thread_proxy(data);

      return 0;
   }
   #else
   static void* pthread_thread_proxy(void* data)
   {
      Thread::details::thread_proxy(data);
      return nullptr;
   }
   #endif

   void Thread::details::thread_proxy(void* data)
   {
      func_wrapper* fn = reinterpret_cast<func_wrapper*>(data);

      (*fn)();

      delete fn;
   }

   Thread::Thread() :
      m_handle((native_handle_t)0)
      #ifdef CORE_WINDOWS
      , m_id((native_id_t)0)
      #endif
   {}

   Thread::Thread(Thread&& other)
   {
      m_handle = std::exchange(other.m_handle, (native_handle_t)0);
      m_id = std::exchange(other.m_id, (native_id_t)0);
   }

   Thread::~Thread()
   {
      if(joinable())
         detach();
   }

   Thread& Thread::operator=(Thread&& other)
   {
      if(this != &other) {
         m_handle = std::exchange(other.m_handle, (native_handle_t)0);
         m_id = std::exchange(other.m_id, (native_id_t)0);
      }

      return *this;
   }

   void Thread::join()
   {
      if(this->joinable()) {
         #ifdef CORE_WINDOWS
         ::WaitForSingleObject(m_handle, INFINITE);
         #else
         ::pthread_join((pthread_t)m_handle, NULL);
         #endif

         detach();
      }
   }

   void Thread::detach()
   {
      if(this->joinable()) {
         #ifdef CORE_WINDOWS
         ::CloseHandle(m_handle);
         m_handle = (native_handle_t)0;
         #else
         ::pthread_detach((pthread_t)m_handle);
         #endif
      }
   }

   bool Thread::joinable() const
   {
      return m_handle != (native_handle_t)0;
   }

   Thread::native_id_t Thread::native_id() const
   {
      #ifdef CORE_WINDOWS
      return m_id;
      #else
      return (native_id_t)m_handle;
      #endif
   }

   void Thread::launch_thread(func_wrapper* fn)
   {
      m_handle = (native_handle_t)0;
      #ifdef CORE_WINDOWS
      static_assert(sizeof(DWORD) == sizeof(native_id_t),
                  "native_id_t no coincide con DWORD");

      m_handle = ::CreateThread(NULL,
                     0,
                     win32_thread_proxy,
                     (LPVOID)fn,
                     CREATE_SUSPENDED,
                     (LPDWORD)&m_id);

      ::ResumeThread(m_handle);
      #else
      pthread_t t;
      if(::pthread_create(&t, NULL, pthread_thread_proxy, fn) == 0)
         m_handle = (native_handle_t)t;
      #endif
   }

   namespace this_thread
   {
      Thread::native_id_t get_id()
      {
         #ifdef CORE_WINDOWS
         return (Thread::native_id_t)::GetCurrentThreadId();
         #else
         return (Thread::native_id_t)::pthread_self();
         #endif
      }  

      void sleep_for(unsigned long milliseconds)
      {
         #ifdef CORE_WINDOWS
         ::Sleep(milliseconds);
         #else
         ::usleep(milliseconds / 1000);
         #endif
      }

      void yield()
      {
         #ifdef CORE_WINDOWS
         ::Sleep(0);
         #elif HAVE_SCHED_YIELD && defined(_POSIX_PRIORITY_SCHEDULING)
         sched_yield();
         #else
         timeval timeout{0};
         select(0, nullptr, nullptr, nullptr, &timeout);
         #endif
      }
   } // namespace this_thread
} // namespace base