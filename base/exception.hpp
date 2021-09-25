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
      template<typename... Args>
      Exception(const char* format, Args&&... args) {
         init(format_to_string(format, std::forward<Args>(args)...));
      }

      char const* what() const noexcept override;
   private:
      void init(const std::string& msg);

      std::string m_msg;
   };
} // namespace base
#endif