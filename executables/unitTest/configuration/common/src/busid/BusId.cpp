// Copyright 2026 Accenture.

#include "busid/BusId.h"

namespace common
{
namespace busid
{

#define BUS_ID_NAME(BUS) \
    case ::busid::BUS: return #BUS

char const* BusIdTraits::getName(uint8_t index)
{
    switch (index)
    {
        BUS_ID_NAME(SELFDIAG);
        BUS_ID_NAME(CAN_0);
        BUS_ID_NAME(ETH_0);
        BUS_ID_NAME(ETH_1);
        default: return "INVALID";
    }
}

} // namespace busid
} // namespace common
