///////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2026 Accenture
//
// This program and the accompanying materials are made available under the
// terms of the Apache License Version 2.0 which is available at
// https://www.apache.org/licenses/LICENSE-2.0
//
// SPDX-License-Identifier: Apache-2.0
////////////////////////////////////////////////////////////////////////////////////

#![allow(static_mut_refs)]

unsafe extern "C" {
    pub fn bsw_cpp_logger_log(component_index: u8, level: Level, str: *const core::ffi::c_char);

    /// Mirror of `::util::logger::Logger::isEnabled`: returns whether the C++ logger
    /// would actually emit an entry for this component/level.
    pub fn bsw_cpp_logger_is_enabled(component_index: u8, level: Level) -> bool;
}

/// Whether the C++ logger would emit an entry for `component_index` at `level`.
///
/// This is the same gate `bsw_cpp_logger_log` applies internally before serializing.
/// Calling it first lets the Rust side skip formatting a message (and the FFI call to
/// log it) when the level is disabled, at the cost of one extra FFI call per log site.
#[inline]
pub fn is_enabled(component_index: u8, level: Level) -> bool {
    // SAFETY: the extern only reads the C++ logger configuration and returns a bool; it
    // has no preconditions and `component_index`/`level` are plain values.
    unsafe { bsw_cpp_logger_is_enabled(component_index, level) }
}

#[unsafe(no_mangle)]
static mut LOGGER: crate::BswLogger = crate::BswLogger::create();

pub(crate) type ComponentIndex = u8;

/// Set the fallback logger component for messages routed through the `log` facade
/// (e.g. plain `log::info!(...)` calls from this crate or third-party deps).
///
/// `component_index` is the C++ `::util::logger::<NAME>` symbol (a `uint8_t`)
/// populated by the logger composition. Pass it directly rather than a name to
/// avoid an unnecessary lookup.
///
/// If this is not called, log messages routed through the `log` facade are silently
/// dropped. Rust code using the per-component `bsw_*!` macros is unaffected.
#[unsafe(no_mangle)]
pub extern "C" fn set_default_component(component_index: u8) {
    // SAFETY: called from C++ during single-threaded startup, before
    // `install_rust_logging` and any other task, so `LOGGER` is not aliased.
    unsafe { LOGGER.set_default_component(component_index) };
}

/// The rust logger must be installed after configuration
#[unsafe(no_mangle)]
pub extern "C" fn install_rust_logging() {
    // SAFETY: called once from C++ during single-threaded startup; `LOGGER` is a
    // `static`, satisfying the `&'static self` the `log` facade requires.
    unsafe { LOGGER.install_logger() };
}

#[allow(dead_code)]
#[allow(clippy::enum_variant_names)]
/// cbindgen:no-export
#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub enum Level {
    LevelDebug = 0,
    LevelInfo = 1,
    LevelWarn = 2,
    LevelError = 3,
    LevelCritical = 4,
    LevelNone = 5,
}
