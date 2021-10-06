#ifndef _BASE_SCOPED_LOCK_HPP
#define _BASE_SCOPED_LOCK_HPP
#include "base/mutex.hpp"

namespace base
{
   class ScopedLock
   {
   public:
      ScopedLock(Mutex& mtx) : m_mtx(mtx) {m_mtx.lock();}
      ~ScopedLock() {m_mtx.unlock();}
   private:
      DISALLOW_COPYING(ScopedLock);
      DISALLOW_MOVING(ScopedLock);

      Mutex& m_mtx;
   };
} // namespace base


#endif