#ifndef _BASE_MUTEX_H
#define _BASE_MUTEX_H
#include "base/base.hpp"

namespace base
{
   class Mutex
   {
   public:
      Mutex();
      ~Mutex();

      void lock() const;
      bool try_lock() const;
      void unlock() const;
      
      DISALLOW_COPYING(Mutex);
   private:
      class MutexImpl;

      mutable MutexImpl* m_impl;
   };
} // namespace base
#endif