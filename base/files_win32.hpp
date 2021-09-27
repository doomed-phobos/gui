#include "base/string.hpp"

#include <windows.h>

namespace base
{
   FILE* open_file_raw(const std::string_view& filename, const std::string_view& mode)
   {
      return _wfopen(from_utf8(filename).c_str(), from_utf8(mode).c_str());
   }
} // namespace base