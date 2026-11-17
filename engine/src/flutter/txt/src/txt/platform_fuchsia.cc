// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <lib/zx/channel.h>
#include <mutex>

#if defined(SK_CODEC_DECODES_PNG_WITH_RUST)
#include "third_party/skia/include/codec/SkCodec.h"
#include "third_party/skia/include/codec/SkPngRustDecoder.h"
#endif

#include "third_party/skia/include/ports/SkFontMgr_fuchsia.h"
#if defined(SK_TYPEFACE_FACTORY_FONTATIONS)
#include "third_party/skia/include/ports/SkFontScanner_Fontations.h"
#else
#include "third_party/skia/include/ports/SkFontScanner_FreeType.h"
#endif
#include "txt/platform.h"

#if defined(SK_FONTMGR_FONTATIONS_AVAILABLE)
#include "third_party/skia/include/ports/SkFontMgr_Fontations.h"
#elif defined(SK_FONTMGR_FREETYPE_EMPTY_AVAILABLE)
#include "third_party/skia/include/ports/SkFontMgr_empty.h"
#endif

namespace txt {

std::vector<std::string> GetDefaultFontFamilies() {
  return {"Roboto"};
}

sk_sp<SkFontMgr> GetDefaultFontManager(uint32_t font_initialization_data) {
#if defined(SK_CODEC_DECODES_PNG_WITH_RUST)
  static std::once_flag register_png_once;
  std::call_once(register_png_once,
                 []() { SkCodecs::Register(SkPngRustDecoder::Decoder()); });
#endif
  if (font_initialization_data) {
    fuchsia::fonts::ProviderSyncPtr sync_font_provider;
    sync_font_provider.Bind(zx::channel(font_initialization_data));
#if defined(SK_TYPEFACE_FACTORY_FONTATIONS)
    return SkFontMgr_New_Fuchsia(std::move(sync_font_provider),
                                 SkFontScanner_Make_Fontations());
#else
    return SkFontMgr_New_Fuchsia(std::move(sync_font_provider),
                                 SkFontScanner_Make_FreeType());
#endif
  } else {
#if defined(SK_FONTMGR_FONTATIONS_AVAILABLE)
    static sk_sp<SkFontMgr> mgr = SkFontMgr_New_Fontations_Empty();
#elif defined(SK_FONTMGR_FREETYPE_EMPTY_AVAILABLE)
    static sk_sp<SkFontMgr> mgr = SkFontMgr_New_Custom_Empty();
#else
    static sk_sp<SkFontMgr> mgr = SkFontMgr::RefEmpty();
#endif
    return mgr;
  }
}

}  // namespace txt
