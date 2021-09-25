#ifndef _BASE_SIGNAL_HPP
#define _BASE_SIGNAL_HPP
#include "base/slot.hpp"

#include <vector>

namespace base
{  
   namespace priv
   {
      template<typename Callable>
      class Signal_base {};

      template<typename R, typename... Args>
      class Signal_base<R(Args...)>
      {
      public:
         typedef Slot<R(Args...)> slot_t;
         typedef std::vector<slot_t*> list_t; // TODO: Reemplazar esto con lista segura

         Signal_base() {}
         Signal_base(const Signal_base& signal) {
            *this = signal;
         }
         Signal_base(Signal_base&& signal) {
            *this = std::move(signal);
         }         
         ~Signal_base() {
            disconnectAll();
         }

         void disconnectAll() {
            for(typename list_t::iterator it = m_slots.begin();
               it != m_slots.end(); ++it)
               delete *it;

            m_slots.clear();
         }

         Signal_base& operator=(Signal_base&& signal) {
            if(this != &signal) {
               copy_from(signal);
               signal.disconnectAll();
            }

            return *this;
         }

         Signal_base& operator=(const Signal_base& signal) {
            if(this != &signal) {
               copy_from(signal);
            }

            return *this;
         }
      private:
         void copy_from(const Signal_base& signal) {
            disconnectAll();
            const list_t& slots = signal.m_slots;
            for(typename list_t::const_iterator it = slots.begin();
               it != slots.end(); ++it) {
               m_slots.push_back((*it)->clone());
            }
         }
      protected:
         slot_t* addSlot(slot_t* slot) {
            m_slots.push_back(slot);
            return slot;
         }

         list_t m_slots;
      };
   } // namespace priv

   template<typename Callable>
   class Signal {};

   template<typename R, typename... Args>
   class Signal<R(Args...)> : public priv::Signal_base<R(Args...)>
   {
   public:
      typedef priv::Signal_base<R(Args...)> INHERITED;
      typedef INHERITED::slot_t slot_t;

      template<typename F, BASE_ENABLE_IF(!std::is_null_pointer_v<F> && std::is_convertible_v<F, slot_t>)>
      slot_t* connect(F&& f) {
         return addSlot(new slot_t(std::forward<F>(f)));
      }

      template<class C,
         typename = typename std::enable_if_t<std::is_class_v<C>>>
      slot_t* connect(C* t, R(C::*mem)(Args...)) {
         return addSlot(new slot_t([=](Args... args) -> R{
            return (t->*mem)(std::forward<Args>(args)...);
         }));
      }

      template<typename... Args2>
      R operator()(Args2&&... args) {
         R result = R();

         for(auto slot : INHERITED::m_slots)
            if(slot)
               result = (*slot)(std::forward<Args2>(args)...);
         
         return result;
      }
   };

   template<typename... Args>
   class Signal<void(Args...)> : public priv::Signal_base<void(Args...)>
   {
   public:
      typedef priv::Signal_base<void(Args...)> INHERITED;
      typedef typename INHERITED::slot_t slot_t;

      template<typename F, BASE_ENABLE_IF(!std::is_null_pointer_v<F> && std::is_convertible_v<F, slot_t>)>
      slot_t* connect(F&& f) {
         return addSlot(new slot_t(std::forward<F>(f)));
      }

      template<class C,
         typename = typename std::enable_if_t<std::is_class_v<C>>>
      slot_t* connect(C* t, void(C::*mem)(Args...)) {
         return addSlot(new slot_t([=](Args... args) {
            (t->*mem)(std::forward<Args>(args)...);
         }));
      }

      template<typename... Args2>
      void operator()(Args2&&... args) {
         for(auto slot : INHERITED::m_slots)
            if(slot)
               (*slot)(std::forward<Args2>(args)...);
      }
   };
} // namespace base
#endif