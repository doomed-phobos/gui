#include "gfx/mask_filter.h"

namespace gfx
{
   static inline SkBlurStyle to_skia(BlurStyle style)
   {
      static_assert((int)kNormal_BlurStyle == (int)kNormal_SkBlurStyle &&
                    (int)kSolid_BlurStyle == (int)kSolid_SkBlurStyle &&
                    (int)kOuter_BlurStyle == (int)kOuter_SkBlurStyle &&
                    (int)kInner_BlurStyle == (int)kInner_SkBlurStyle, "BlurStyle no coincide a SkBlurStyle");
      
      return (SkBlurStyle)style;
   }

   sk_sp<SkMaskFilter> MaskFilterFactory::MakeBlur(BlurStyle style, float sigma,
                                                   bool respectCTM)
   {
      return SkMaskFilter::MakeBlur(to_skia(style), sigma, respectCTM);
   }
} // namespace gfx