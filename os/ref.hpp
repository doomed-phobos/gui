#ifndef _OS_REF_HPP
#define _OS_REF_HPP
#include "base/shared_ptr.hpp"
#include "base/referenceable.hpp"

namespace os
{
   template<typename T>
   using SharedPtr = base::SharedPtr<T>;

   typedef base::Referenceable Referenceable;
} // namespace os

#endif