#include "base/files.hpp"

#ifdef CORE_WINDOWS
#  include "base/files_win32.hpp"
#endif

namespace base
{
   size_t get_file_position(FILE* file)
   {
      long curr = ftell(file);
      if(curr < 0)
         return 0;
      
      return curr;
   }

   size_t get_file_size(FILE* file)
   {
      if(!file)
         return 0;
      
      long curr = ftell(file);
      if(curr < 0)
         return 0;
      
      fseek(file, 0, SEEK_END);
      long size = ftell(file);
      if(size < 0)
         size = 0;

      fseek(file, curr, SEEK_SET);
      return size;
   }
} // namespace base