#ifndef _BASE_REFERENCEABLE_HPP
#define _BASE_REFERENCEABLE_HPP
#include "base/base.hpp"
#include "base/debug.hpp"

#include <atomic>

namespace base
{
   class Referenceable
   {
   public:
      Referenceable() : m_refCnt(1) {}
      virtual ~Referenceable() {
         m_refCnt.store(0, std::memory_order_relaxed);
      }

      void ref() const {
         ASSERT(use_count() > 0);
         m_refCnt.fetch_add(1, std::memory_order_relaxed);
      }

      void unref() const {
         ASSERT(use_count() > 0);
         if(m_refCnt.fetch_add(-1, std::memory_order_acq_rel) == 1)
            delete this;
      }
   private:
      template<typename>
      friend class SharedPtr;

      DISALLOW_COPYING(Referenceable);
      DISALLOW_MOVING(Referenceable);

      int32_t use_count() const {
         return m_refCnt.load(std::memory_order_relaxed);
      }

      mutable std::atomic_int32_t m_refCnt;
   };
} // namespace base
#endif