/// Use debug mode to view leaks. Only MSVC
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include "base/mutex.hpp"
#include "base/shared_ptr.hpp"
#include "base/referenceable.hpp"
#include "base/signal.hpp"
#include "base/thread.hpp"

#include <vector>

class A : public base::Referenceable {};

int main()
{
   _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
   base::Mutex mtx;
   base::SharedPtr<A> pA(new A());
   {
      auto pB = std::move(pA);
   }
   base::Signal<void()> signalA;
   for(int i = 0; i < 1e3; ++i)
      signalA.connect([]{});
   auto signalB = std::move(signalA);
   {
      auto signalC = std::move(signalB);
   }
   
   {
      std::vector<base::Thread> v;
      for(int i = 0; i < 1e3; ++i)
         v.emplace_back([]{});
   }   

   return 0;
}