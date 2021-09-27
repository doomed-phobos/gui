#ifndef _BASE_SLOT_HPP
#define _BASE_SLOT_HPP
#include "base/base.hpp"

#include <functional>

namespace base
{
   namespace priv
   {
      class Slot_base
      {
      public:
         Slot_base() {}
         virtual ~Slot_base() {}

         Slot_base(const Slot_base&) = delete;
         Slot_base& operator=(const Slot_base&) = delete;
      };
   } // namespace priv

   template<typename Callable>
   class Slot {};

   template<typename R, typename... Args>
   class Slot<R(Args...)> : public priv::Slot_base
   {
   public:
      template<typename F, BASE_ENABLE_IF(std::is_convertible_v<F, std::function<R(Args...)>>)>
      Slot(F&& fn) : m_function(std::forward<F>(fn)) {}

      Slot* clone() const {
         return new Slot(m_function);
      }

      template<typename... Args2>
      R operator()(Args2&&... args) {
         return m_function(std::forward<Args2>(args)...);
      }

      explicit operator bool() const {
         return m_function != nullptr;
      }
   private:
      std::function<R(Args...)> m_function;
   };

   template<typename... Args>
   class Slot<void(Args...)> : public priv::Slot_base
   {
   public:
      template<typename F, BASE_ENABLE_IF(std::is_convertible_v<F, std::function<void(Args...)>>)>
      Slot(F&& fn) : m_function(std::forward<F>(fn)) {}

      Slot* clone() const {
         return new Slot(m_function);
      }

      template<typename... Args2>
      void operator()(Args2&&... args) {
         m_function(std::forward<Args2>(args)...);
      }

      explicit operator bool() const {
         return m_function != nullptr;
      }
   private:
      std::function<void(Args...)> m_function;
   };
} // namespace base
#endif