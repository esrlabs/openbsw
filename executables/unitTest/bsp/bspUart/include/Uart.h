// Copyright 2025 BMW AG

#pragma once

#include "bsp/uart/UartApi.h"

namespace bsp
{

class Uart : public bsp::UartApi
{
public:
    size_t write(uint8_t const* data, size_t length);

    size_t read(uint8_t* data, size_t length);

    void init();
};

} // namespace bsp
