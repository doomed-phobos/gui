#ifndef _BASE_SHARED_PTR_HPP
#define _BASE_SHARED_PTR_HPP
#include "base/types.hpp"
#include "base/debug.hpp"

#include <type_traits>

namespace base
{
   template<typename T>
   static inline T* ref(T* obj)
   {
      ASSERT(obj);
      obj->ref();
      return obj;
   }

   template<typename T>
   static inline T* safe_ref(T* obj)
   {
      if(obj)
         obj->ref();
         
      return obj;
   }

   template<typename T>
   static inline T* safe_unref(T* obj)
   {
      if(obj)
         obj->unref();
         
      return obj;
   }

   template<typename T>
   class SharedPtr
   {
   public:
      SharedPtr() : m_ptr(nullptr) {}
      SharedPtr(std::nullptr_t) : m_ptr(nullptr) {}
      explicit SharedPtr(T* ptr) : m_ptr(ptr) {}
      
      template<typename U,
         typename = typename std::enable_if_t<std::is_convertible_v<U*, T*>>>
      SharedPtr(const SharedPtr<U>& that) : m_ptr(safe_ref(that.get())) {}
      SharedPtr(const SharedPtr& sp) : 
         m_ptr(safe_ref(sp.get())) {}

      SharedPtr(SharedPtr&& sp) :
         m_ptr(sp.release()) {}
      template<typename U,
         typename = typename std::enable_if_t<std::is_convertible_v<U*, T*>>>
      SharedPtr(SharedPtr<U>&& that) : m_ptr(that.release()) {}

      ~SharedPtr() {
         safe_unref(m_ptr);
      }

      void reset(T* ptr = nullptr) {
         T* oldPtr = m_ptr;
         m_ptr = ptr;
         safe_unref(oldPtr);
      }

      T* release() {
         T* ptr = m_ptr;
         m_ptr = nullptr;
         return ptr;
      }

      int32_t use_count() const {
         return m_ptr ? m_ptr->use_count() : 0;
      }

      T& operator*() const {
         ASSERT(m_ptr != nullptr);
         return *m_ptr;
      }

      explicit operator bool() const {return m_ptr != nullptr;}
      T* get() const {return m_ptr;}
      T* operator->() const {return m_ptr;}

      SharedPtr& operator=(std::nullptr_t) {
         reset();
         return *this;
      }
      SharedPtr& operator=(const SharedPtr& sp) {
         if(this != &sp)
            reset(safe_ref(sp.get()));

         return *this;
      }
      SharedPtr& operator=(SharedPtr&& sp) {
         if(this != &sp)
            reset(sp.release());
         
         return *this;
      }
   private:
      T* m_ptr;
   };

   template<typename T, typename U>
   inline bool operator==(const SharedPtr<T>& a, const SharedPtr<U>& b)
   {
      return a.get() == b.get();
   }

   template<typename T>
   inline bool operator==(const SharedPtr<T>&a, std::nullptr_t)
   {
      return !a;
   }

   template<typename T, typename U>
   inline bool operator!=(const SharedPtr<T>& a, const SharedPtr<U>& b)
   {
      return a.get() != b.get();
   }

   template<typename T>
   inline bool operator!=(const SharedPtr<T>& a, std::nullptr_t)
   {
      return static_cast<bool>(a);
   }

   template<typename To, typename From>
   SharedPtr<To> dynamic_pointer_cast(const SharedPtr<From>& other)
   {
      const auto ptr = dynamic_cast<To*>(other.get());

      if(ptr)
         return SharedPtr<To>(safe_ref(ptr));
      
      return SharedPtr<To>();
   }

   template<typename T, typename... Args>
   SharedPtr<T> make_shared(Args&&... args)
   {
      return SharedPtr<T>(new T(std::forward<Args>(args)...));
   }
} // namespace base
#endif