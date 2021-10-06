#ifndef _CORE_HPP
#define _CORE_HPP

// Detect config
#ifdef _DEBUG
#  define CORE_DEBUG
#endif

#ifdef _MSC_VER
#  define CORE_MSC
#endif

// Detect os
#if defined(_WIN32)
#  define CORE_WINDOWS
#elif defined(__APPLE__)
#  define CORE_MACOS
#else
#  define CORE_LINUX
#endif

// Os config
#if defined(CORE_WINDOWS)
#  define NOMINMAX
#  define UNICODE
#endif

// Compiler config
#ifdef CORE_MSC
#    pragma warning(disable:4996)
#endif

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#  define CORE_CPU_BENDIAN
#elif defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#  define CORE_CPU_LENDIAN
#elif defined(__sparc) || defined(__sparc__) || \
      defined(_POWER) || defined(__powerpc__) || \
      defined(__ppc__) || defined(__hppa) || \
      defined(__PPC__) || defined(__PPC64__) || \
      defined(_MIPSEB) || defined(__ARMEB__) || \
      defined(__s390__) || \
      (defined(__sh__) && defined(__BIG_ENDIAN__)) || \
      (defined(__ia64) && defined(__BIG_ENDIAN__))
#  define CORE_CPU_BENDIAN
#else
#  define CORE_CPU_LENDIAN
#endif

#endif
