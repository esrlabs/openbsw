///////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2026 Accenture
//
// This program and the accompanying materials are made available under the
// terms of the Apache License Version 2.0 which is available at
// https://www.apache.org/licenses/LICENSE-2.0
//
// SPDX-License-Identifier: Apache-2.0
////////////////////////////////////////////////////////////////////////////////////

pub struct LogBuf<const LOG_BUF_SIZE: usize> {
    buffer: [u8; LOG_BUF_SIZE],
    bytes_used: usize,
}

impl<const LOG_BUF_SIZE: usize> LogBuf<LOG_BUF_SIZE> {
    pub fn as_raw_pointer(&self) -> *const core::ffi::c_char {
        &self.buffer as *const u8 as *const core::ffi::c_char
    }
}

impl<const LOG_BUF_SIZE: usize> Default for LogBuf<LOG_BUF_SIZE> {
    fn default() -> Self {
        Self {
            buffer: [0; LOG_BUF_SIZE],
            bytes_used: 0,
        }
    }
}

impl<const LOG_BUF_SIZE: usize> core::fmt::Write for LogBuf<LOG_BUF_SIZE> {
    fn write_str(&mut self, s: &str) -> core::fmt::Result {
        let bytes = s.as_bytes();
        // always keep at least one trailing null byte
        let remaining = LOG_BUF_SIZE - self.bytes_used - 1;
        let n = bytes.len().min(remaining);
        self.buffer[self.bytes_used..self.bytes_used + n].copy_from_slice(&bytes[..n]);
        self.bytes_used += n;
        if n == bytes.len() {
            Ok(())
        } else {
            // not enough space, the message was truncated
            Err(core::fmt::Error)
        }
    }
}

#[cfg(test)]
mod test {
    use crate::log_buf::LogBuf;
    use core::fmt::Write;

    #[test]
    fn test_format_into_logbuf_until_full() {
        const BUF_SIZE: usize = 20;
        let mut buf = LogBuf::<BUF_SIZE>::default();
        assert!(write!(buf, "").is_ok());
        assert_eq!(buf.bytes_used, 0);
        assert!(buf.buffer.starts_with(b"\0"));

        assert!(write!(buf, "Hello World!").is_ok());
        assert_eq!(buf.bytes_used, "Hello World!".len());
        assert!(buf.buffer.starts_with(b"Hello World!\0"));

        assert!(write!(buf, "Hello World!").is_err());
        assert_eq!(buf.bytes_used, BUF_SIZE - 1);
        assert!(buf.buffer.starts_with(b"Hello World!Hello W\0"));
    }

    #[test]
    fn test_format_into_logbuf_direct_full() {
        const BUF_SIZE: usize = 20;
        let mut buf = LogBuf::<BUF_SIZE>::default();
        assert!(write!(buf, "Hello World!Hello World!").is_err());
        assert_eq!(buf.bytes_used, BUF_SIZE - 1);
        assert!(buf.buffer.starts_with(b"Hello World!Hello W\0"));
    }

    #[test]
    fn test_format_into_logbuf_one_byte_to_large() {
        const BUF_SIZE: usize = 20;
        let mut buf = LogBuf::<BUF_SIZE>::default();
        assert!(write!(buf, "Hello World!Hello Wo").is_err());
        assert_eq!(buf.bytes_used, BUF_SIZE - 1);
        assert!(buf.buffer.starts_with(b"Hello World!Hello W\0"));
    }

    #[test]
    fn test_format_into_logbuf_just_fits() {
        const BUF_SIZE: usize = 20;
        let mut buf = LogBuf::<BUF_SIZE>::default();
        assert!(write!(buf, "Hello World!Hello W").is_ok());
        assert_eq!(buf.bytes_used, BUF_SIZE - 1);
        assert!(buf.buffer.starts_with(b"Hello World!Hello W\0"));
    }
}
