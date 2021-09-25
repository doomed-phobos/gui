#include "base/string.hpp"

#include <algorithm>

namespace
{
   template<typename Char>
   struct is_separator
   {
   public:
      is_separator(const Char separator) :
         m_separator(separator) {}

      bool operator()(const Char chr) const {
         return chr == m_separator;
      }
   private:
      Char m_separator;
   };
}

namespace base
{
   // Based on SKIA code (skia/src/utils/SKUTF.cpp)
   static constexpr inline int32_t left_shift(int32_t value, int32_t shift)
   {
      return (int32_t) ((uint32_t) value << shift);
   }

   /** @returns  -1  iff invalid UTF8 byte,
                  0  iff UTF8 continuation byte,
                  1  iff ASCII byte,
                  2  iff leading byte of 2-byte sequence,
                  3  iff leading byte of 3-byte sequence, and
                  4  iff leading byte of 4-byte sequence.
         I.e.: if return value > 0, then gives length of sequence.
   */
   static int utf8_byte_type(uint8_t c)
   {
      if(c < 0x80)
         return 1;
      else if(c < 0xC0)
         return 0;
      else if(c >= 0xF5 || (c & 0xFE) == 0xC0)
         return -1;
      else
         return (((0xe5 << 24) >> ((unsigned)c >> 4 << 1)) & 3) + 1;
   }
   static bool utf8_type_is_valid_leading_byte(int type) { return type > 0;}

   static bool utf8_byte_is_continuation(uint8_t c) { return utf8_byte_type(c) == 0;}

   static int next_fail(const char*& ptr, const char* end)
   {
      ptr = end;
      return -1;
   }

   static int next_utf8(const char*& ptr, const char* end)
   {
      if(!ptr || !end)
         return -1;

      const uint8_t*  p = (const uint8_t*)ptr;
      if(!p || p >= (const uint8_t*)end)
         return next_fail(ptr, end);

      int c = *p;
      int hic = c << 24;

      if(!utf8_type_is_valid_leading_byte(utf8_byte_type(c)))
         return next_fail(ptr, end);
      
      if(hic < 0) {
         uint32_t mask = (uint32_t)~0x3F;
         hic = left_shift(hic, 1);
         do {
            ++p;
            if(p >= (const uint8_t*)end)
               return next_fail(ptr, end);
            
            // check before reading off end of array.
            uint8_t nextByte = *p;
            if(!utf8_byte_is_continuation(nextByte))
               return next_fail(ptr, end);
            
            c = (c << 6) | (nextByte & 0x3F);
            mask <<= 5;
         } while((hic = left_shift(hic, 1)) < 0);
         c &= ~mask;
      }

      ptr = (char*)p + 1;
      return c;
   }

   std::wstring from_utf8(const char* src, size_t size)
   {
      const char* ptr = src;
      const char* end = src + size;
      size_t n = 0;
      while(ptr < end) {
         int u = next_utf8(ptr, end);
         if(u < 0)
            return std::wstring();

         n++;
      }

      std::vector<wchar_t> buf(n + 1);
      ptr = src;
      wchar_t* out = buf.data();
      while(ptr < end)
         *out++ = next_utf8(ptr, end);
      *out = 0;

      return std::wstring(&buf[0]);
   }

   // Based on Allegro Unicode code (allegro/src/unicode.c)
   static size_t insert_utf8_char(std::string* res, wchar_t wchr)
   {
      if(wchr < 0x80) {
         if(res)
            res->push_back((char)wchr);
         return 1;
      }

      int bits = 7;
      while(wchr >= (1 << bits))
         bits++;
      
      size_t size = 2, b = 11;
      while(b < bits) {
         size++;
         b += 5;
      }
      
      if(res) {
         b -= (7 - size);
         int firstByte = wchr >> b;
         for(int i = 0; i < size; ++i)
            firstByte |= (0x80 >> i);
         
         res->push_back(firstByte);

         for(int i = 1; i < size; ++i) {
            b -= 6;
            res->push_back((0x80 | (wchr >> b) & 0x3F));
         }
      }

      return size;
   }

   std::string to_utf8(const wchar_t* src, size_t size)
   {
      const wchar_t* ptr = src;
      const wchar_t* end = src + size;
      size_t n = 0;
      while(ptr < end)
         n += insert_utf8_char(nullptr, *ptr++);

      std::string res;
      res.reserve(n+1);
      ptr = src;     
      while(ptr < end)
         insert_utf8_char(&res, *ptr++);

      return res;
   }

   template<typename Char, typename String = typename std::basic_string<Char>>
   void split_tstring(const String& str, std::vector<String>& parts, const Char separator)
   {
      size_t total_elements = std::count_if(str.begin(), str.end(), is_separator(separator)) + 1;
      parts.reserve(total_elements);

      String::size_type beg = 0, end;
      while(true) {
         end = str.find_first_of(separator, beg);
         if(end != String::npos) {
            parts.push_back(str.substr(beg, end-beg));
            beg = end+1;
         }else {
            parts.push_back(str.substr(beg));
            break;
         }
      }
   }

   void split_string(const std::string& str, std::vector<std::string>& parts, const char separator)
   {
      split_tstring(str, parts, separator);
   }
   void split_string(const std::wstring& wstr, std::vector<std::wstring>& parts, const wchar_t separator)
   {
      split_tstring(wstr, parts, separator);
   }
} // namespace base