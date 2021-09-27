#ifndef _BASE_ONCE_H
#define _BASE_ONCE_H
#include <utility>

namespace base
{
   class Once
   {
   public:
      constexpr Once() = default;

      template<typename Fn, typename... Args>
      void operator()(Fn&& fn, Args&&... args) {
         if(m_runned)
            return;

         fn(std::forward<Args>(args)...);

         m_runned = true;
      }
   private:
      bool m_runned{false};
   };
} // namespace base

#endif