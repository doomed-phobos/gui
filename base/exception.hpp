#ifndef _BASE_EXCEPTION_HPP
#define _BASE_EXCEPTION_HPP
#include "base/string.hpp"

#include <exception>

namespace base
{
   class Exception : public std::exception
   {
   public:
      Exception();
      Exception(const std::string& msg);
      Exception(const char* format, ...);

      char const* what() const noexcept override;
   private:
      void init(const std::string& msg);

      std::string m_msg;
   };
} // namespace base
#endif