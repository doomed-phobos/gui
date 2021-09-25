#ifndef _BASE_THREAD_H
#define _BASE_THREAD_H
#include "base/types.hpp"

#include <tuple>
#include <utility>

namespace base
{
   namespace priv
   {
      template<size_t...> struct seq {};
      template<size_t N, size_t... S> struct gen : public gen<N-1, N-1, S...> {};
      template<size_t... S> struct gen<0, S...>
      {
         typedef typename seq<S...> type;
      };

      template<typename Fn, typename Tuple, size_t... S>
      static void apply_tuple(Fn&& fn, const Tuple& tuple, const seq<S...>)
      {
         fn(std::get<S>(tuple)...);
      }
   } // namespace priv

   class Thread
   {
   public:
      typedef void* native_handle_t;
      #ifdef CORE_WINDOWS
      typedef uint32_t native_id_t;
      #else
      typedef void* native_id_t;
      #endif   

      Thread();
      template<typename Fn, typename... Args>
      explicit Thread(Fn&& fn, Args&&... args) {
         launch_thread(new func_wrapperN<Fn, Args...>(std::forward<Fn>(fn), std::forward<Args>(args)...));
      }
      Thread(Thread&& other);
      ~Thread();

      void join();
      void detach();

      bool joinable() const;

      native_id_t native_id() const;

      class details {
      public:
         static void thread_proxy(void* data);
      };
      
      Thread& operator=(Thread&& other);
   private:
      DISALLOW_COPYING(Thread);

      class func_wrapper
      {
      public:
         func_wrapper() {}
         virtual void operator()() = 0;
      };

      template<typename Fn, typename... Args>
      class func_wrapperN : public func_wrapper
      {
      public:
         typedef typename std::tuple<Args&&...> tuple_t;

         func_wrapperN(Fn&& fn, Args&&... args) :
            m_fn(std::forward<Fn>(fn)) ,
            m_args(std::forward<Args>(args)...) {}
         
         virtual void operator()() override {
            priv::apply_tuple(std::forward<Fn>(m_fn),
               m_args,
               typename priv::gen<sizeof...(Args)>::type());
         }
      private:
         Fn&& m_fn;
         tuple_t m_args;
      };

      void launch_thread(func_wrapper* fn);

      native_handle_t m_handle;
      native_id_t m_id;
   };

   namespace this_thread
   {
      Thread::native_id_t get_id();
      void sleep_for(unsigned long milliseconds);  
      void yield();
   } // namespace this_thread
} // namespace base

#endif