#ifndef _BASE_TYPES_HPP
#define _BASE_TYPES_HPP
#include "base/config.hpp"

#if HAVE_STDINT_H
#  include "cstdint"
#else
#  error "uint8_t, uint32_t, etc. definitions are missing"
#endif

#define DISALLOW_COPYING(classname)                \
   classname(const classname&) = delete;           \
   classname& operator=(const classname&) = delete;

#define DISALLOW_MOVING(classname)            \
   classname(classname&&) = delete;           \
   classname& operator=(classname&&) = delete;

#endif