// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "txt/platform.h"

#include <mutex>

#if defined(SK_CODEC_DECODES_PNG_WITH_RUST)
#include "third_party/skia/include/codec/SkCodec.h"
#include "third_party/skia/include/codec/SkPngRustDecoder.h"
#endif

#if defined(SK_FONTMGR_FONTATIONS_AVAILABLE)
#include "third_party/skia/include/ports/SkFontMgr_Fontations.h"
#elif defined(SK_FONTMGR_FREETYPE_EMPTY_AVAILABLE)
#include "third_party/skia/include/ports/SkFontMgr_empty.h"
#endif

namespace txt {

std::vector<std::string> GetDefaultFontFamilies() {
  return {"Arial"};
}

sk_sp<SkFontMgr> GetDefaultFontManager(uint32_t font_initialization_data) {
#if defined(SK_CODEC_DECODES_PNG_WITH_RUST)
  static std::once_flag register_png_once;
  std::call_once(register_png_once,
                 []() { SkCodecs::Register(SkPngRustDecoder::Decoder()); });
#endif
#if defined(SK_FONTMGR_FONTATIONS_AVAILABLE)
  static sk_sp<SkFontMgr> mgr = SkFontMgr_New_Fontations_Empty();
#elif defined(SK_FONTMGR_FREETYPE_EMPTY_AVAILABLE)
  static sk_sp<SkFontMgr> mgr = SkFontMgr_New_Custom_Empty();
#else
  static sk_sp<SkFontMgr> mgr = SkFontMgr::RefEmpty();
#endif
  return mgr;
}

}  // namespace txt
