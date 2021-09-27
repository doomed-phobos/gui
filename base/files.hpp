#ifndef _BASE_FILES_HPP
#define _BASE_FILES_HPP
#include <cstdio>
#include <memory>
#include <string>

namespace base
{
   typedef std::shared_ptr<FILE> FILEHandle;

   FILE* open_file_raw(const std::string_view& filename, const std::string_view& mode);
   size_t get_file_position(FILE* file);
   size_t get_file_size(FILE* file);
} // namespace base

#endif