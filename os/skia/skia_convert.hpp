#ifndef _OS_SKIA_SKIA_CONVERT_HPP
#define _OS_SKIA_SKIA_CONVERT_HPP
#include "os/text_align.hpp"

#include <SkTextUtils.h>

namespace os
{
   inline SkTextUtils::Align to_skia(const TextAlign align)
   {
      static_assert((int)kLeft_TextAlign == (int)SkTextUtils::kLeft_Align &&
                    (int)kCenter_TextAlign == (int)SkTextUtils::kCenter_Align &&
                    (int)kRight_TextAlign == (int)SkTextUtils::kRight_Align,
                    "os::TextAlign don't match with SkTextUtils::Align");

      return (SkTextUtils::Align)align;
   }
} // namespace os
#endif