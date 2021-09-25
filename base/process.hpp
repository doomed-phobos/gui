#ifndef _BASE_PROCESS_HPP
#define _BASE_PROCESS_HPP
#include "base/types.hpp"

namespace base
{
   typedef uint32_t pid_t;

   pid_t get_current_process_id();
} // namespace base

#endif