// Copyright 2024 Accenture.
// Copyright 2025 BMW AG

#include "bsp/Uart.h"

#include <io/Io.h>
#include <mcu/mcu.h>

namespace bsp
{

struct UartBaudRate
{
    uint32_t baud;
};

struct Uart::UartDevice
{
    LPUART_Type& uart;
    bios::Io::PinId txPin;
    bios::Io::PinId rxPin;
    uint8_t speedModes;
    UartBaudRate const* baudRate;
};

} // namespace bsp
