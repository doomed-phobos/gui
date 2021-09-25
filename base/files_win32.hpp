#include "base/string.hpp"
#include <windows.h>

namespace base
{
   FILE* open_file_raw(const std::string& filename, const std::string& mode)
   {
      return _wfopen(from_utf8(filename).c_str(), from_utf8(mode).c_str());
   }
} // namespace base