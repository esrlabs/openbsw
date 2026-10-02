# *******************************************************************************
# Copyright (c) 2026 Accenture
#
# This program and the accompanying materials are made available under the
# terms of the Apache License Version 2.0 which is available at
# https://www.apache.org/licenses/LICENSE-2.0
#
# SPDX-License-Identifier: Apache-2.0
# *******************************************************************************

set -euo pipefail

readonly ELF=/firmware/build/s32k148-freertos-gcc/executables/referenceApp/application/RelWithDebInfo/app.referenceApp.elf
readonly GDB_SERVER=host.containers.internal:7224

printf '\nFlashing %s via the native host GDB server at %s...\n\n' "$ELF" "$GDB_SERVER"

exec /opt/arm-gnu-toolchain/bin/arm-none-eabi-gdb \
    --batch \
    --eval-command="target remote $GDB_SERVER" \
    --eval-command=load \
    --eval-command=quit \
    "$ELF"
