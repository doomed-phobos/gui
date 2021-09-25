#include "base/mutex.hpp"

#ifdef CORE_WINDOWS
#  include "base/mutex_win32.hpp"
#else
#  error "Implementar esto"
#endif

namespace base
{
   Mutex::Mutex() :
      m_impl(new MutexImpl)
   {
   }

   Mutex::~Mutex()
   {
      delete m_impl;
   }

   void Mutex::lock() const
   {
      m_impl->lock();
   }

   bool Mutex::try_lock() const
   {
      return m_impl->try_lock();
   }

   void Mutex::unlock() const
   {
      m_impl->unlock();
   }
} // namespace base