// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Unified Rust staticlib root crate for the Flutter engine.
// Linking all Rust FFI crates (`flutter_rust`, `skia_rust_png_ffi`, and
// `skia_fontations_ffi`) through a single `staticlib` target allows `rustc` to
// perform whole-program LTO (`-Clto=fat`) and dead-code elimination across all
// Rust crates and `libstd` without duplicate allocator or runtime symbols.

extern crate flutter_rust;

#[cfg(skia_rust_png)]
extern crate skia_rust_png_ffi;

#[cfg(skia_fontations)]
extern crate skia_fontations_ffi;
