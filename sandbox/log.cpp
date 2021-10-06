#include "base/debug.hpp"
#include "base/log.hpp"
#include "base/thread.hpp"

#include <vector>

int main()
{
   std::vector<base::Thread> vt;
   for(int i = 0; i < 50; ++i)
      vt.emplace_back([]{LOG_INFO("{}", base::this_thread::get_id());});

   for(auto& t : vt)
      t.join();

   return 0;
}