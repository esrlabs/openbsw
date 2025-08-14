// Copyright 2024 Accenture.
// Copyright 2025 BMW AG

#include "bsp/UartPrivate.h"

#include <bsp/clock/boardClock.h>
#include <util/estd/assert.h>

using bsp::Uart;

void Uart::init()
{
    (void)bios::Io::setDefaultConfiguration(_uartDevice.txPin);
    (void)bios::Io::setDefaultConfiguration(_uartDevice.rxPin);
    _uartDevice.uart.GLOBAL = 0;
    _uartDevice.uart.CTRL   = 0;

    _uartDevice.uart.PINCFG = 0;
    _uartDevice.uart.BAUD   = 0;
    _uartDevice.uart.BAUD   = _uartDevice.baudRate[0].baud;
    _uartDevice.uart.STAT   = 0xFFFFFFFFU;
    _uartDevice.uart.STAT   = 0;
    _uartDevice.uart.MODIR  = 0;

    _uartDevice.uart.FIFO  = 0;
    _uartDevice.uart.WATER = 0;
    // Last
    _uartDevice.uart.CTRL  = LPUART_CTRL_RE(1U) + LPUART_CTRL_TE(1U);
}

bool Uart::isRxReady() const
{
    if ((_uartDevice.uart.STAT & LPUART_STAT_OR_MASK) != 0)
    {
        _uartDevice.uart.STAT = _uartDevice.uart.STAT | LPUART_STAT_OR_MASK;
    }
    return ((_uartDevice.uart.STAT & LPUART_STAT_RDRF_MASK) != 0);
}

size_t Uart::read(uint8_t* data, size_t length)
{
    size_t bytes_read = 0;

    if (data == nullptr)
    {
        return 0;
    }

    while (isRxReady() && (bytes_read < length))
    {
        data[bytes_read] = _uartDevice.uart.DATA & 0xFFU;
        bytes_read++;
    }

    return bytes_read;
}

bool Uart::isTxActive() const { return ((_uartDevice.uart.STAT & LPUART_STAT_TDRE_MASK) == 0); }

size_t Uart::write(uint8_t const* data, size_t length)
{
    size_t counter = 0;

    if (data == nullptr)
    {
        return 0;
    }

    while (counter < length)
    {
        if (!writeByte(data[counter]))
        {
            break;
        }
        counter++;
    }

    return counter;
}

bool Uart::writeByte(uint8_t data)
{
    if (!isTxActive())
    {
        _uartDevice.uart.DATA = (static_cast<uint32_t>(data) & 0xFFU);
        while (isTxActive()) {}
        return true;
    }
    return false;
}
