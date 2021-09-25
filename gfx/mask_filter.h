#ifndef _GFX_MASK_H
#define _GFX_MASK_H
#include "SkMaskFilter.h"

namespace gfx
{
   enum BlurStyle
   {
      kNormal_BlurStyle,
      kSolid_BlurStyle,
      kOuter_BlurStyle,
      kInner_BlurStyle
   };

   class MaskFilterFactory
   {
   public:
      static sk_sp<SkMaskFilter> MakeBlur(BlurStyle style, float sigma,
                                bool respectCTM = true);
   private:
      MaskFilterFactory() = delete;
   };
} // namespace gfx
#endif