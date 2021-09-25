//////////////////////////////////////////////////////////////////////
/// Copyright (c) 2021 surreal-Ceres
/// 
/// Este archivo debe ser usado junto con "signal.h"
/// 
/// Esto es un archivo creado por mí
/// @code
/// Codigo de ejemplo aqui
/// @endcode
//////////////////////////////////////////////////////////////////////

#ifndef _SLOT_HPP
#define _SLOT_HPP
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

   /// @class Una ranura que debe contener el formato tipo_de_funcion(argumentos...)
   template<typename Callable>
   class Slot {};

   // Una ranura cuya función
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