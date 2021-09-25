#ifndef _BASE_STRING_HPP
#define _BASE_STRING_HPP
#include <string>
#include <cstdarg>
#include <vector>

namespace base
{
   /// Convierte un arreglo UNICODE a ANSI (wchar_t -> char).
   /// @param src El arreglo UNICODE a transformar.
   /// @param size Tamaño del arreglo. Si es menor que el tamaño del arreglo, esta será cortada; en cambio,
   ///             si es mayor, no se cortará ni aumentará.
   /// @return Una cadena ANSI.
   std::string to_utf8(const wchar_t* src, size_t size);
   /// Convierte una cadena UNICODE a ANSI (std::wstring -> std::string).
   /// @param wstr La cadena UNICODE a transformar.
   /// @return Una cadena ANSI.
   /// @see base::to_utf8(const wchar_t* src, const int size)
   inline std::string to_utf8(const std::wstring_view& wstr) {
      return to_utf8(wstr.data(), wstr.size());
   }

   /// Convierte un arreglo ANSI a UNICODE (char -> wchar_t).
   /// @param src El arreglo ANSI a transformar.
   /// @param size Tamaño del arreglo. Si es menor que el tamaño del arreglo, esta será cortada; en cambio,
   ///             si es mayor, no se cortará ni aumentará.
   /// @return Una cadena UNICODE.
   std::wstring from_utf8(const char* src, size_t size);
   /// Convierte una cadena ANSI a UNICODE (std::string -> std::wstring).
   /// @param wstr La cadena ANSI a transformar.
   /// @return Una cadena UNICODE.
   /// @see base::from_utf8(const char* src, const int size)
   inline std::wstring from_utf8(const std::string_view& str) {
      return from_utf8(str.data(), str.size());
   }

   template<typename... Args>
   std::string format_to_string(const char* format, Args&&... args);
   template<typename... Args>
   std::wstring format_to_string(const wchar_t* format, Args&&... args);

   void split_string(const std::string& str, std::vector<std::string>& parts, const char separator);
   void split_string(const std::wstring& wstr, std::vector<std::wstring>& parts, const wchar_t separator);
} // namespace base

#include "base/string-inl.hpp"

#endif