// Copyright 2024 Accenture.

#pragma once

#include <etl/optional.h>
#include <etl/span.h>

#include <array>

namespace transport
{

struct LogicalAddress
{
    uint16_t address2Byte;
    uint16_t address1Byte;
};

namespace addressfinder
{
::etl::optional<LogicalAddress>
find2ByteAddressInSlice(uint16_t address, ::etl::span<LogicalAddress const> const& list);

::etl::optional<LogicalAddress>
find1ByteAddressInSlice(uint16_t address, ::etl::span<LogicalAddress const> const& list);

inline bool is2ByteAddressIn(uint16_t const address, ::etl::span<LogicalAddress const> const& list)
{
    return find2ByteAddressInSlice(address, list).has_value();
}

inline bool is1ByteAddressIn(uint16_t const address, ::etl::span<LogicalAddress const> const& list)
{
    return find1ByteAddressInSlice(address, list).has_value();
}
} // namespace addressfinder

template<size_t N>
class LogicalAddressConverter
{
public:
    static uint16_t convert2ByteAddressTo1Byte(uint16_t const address)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wundefined-var-template"
        for (auto& l : TESTER_ADDRESS_LISTS)
#pragma GCC diagnostic pop
        {
            auto ret = addressfinder::find2ByteAddressInSlice(address, l);
            if (ret.has_value())
            {
                return ret->address1Byte;
            }
        }
        return address;
    }

    static uint16_t convert1ByteAddressTo2Byte(uint16_t const address)
    {
        for (auto& l : TESTER_ADDRESS_LISTS)
        {
            auto ret = addressfinder::find1ByteAddressInSlice(address, l);
            if (ret.has_value())
            {
                return ret->address2Byte;
            }
        }
        return address;
    }

    static etl::array<::etl::span<LogicalAddress const>, N> const TESTER_ADDRESS_LISTS;
};
} // namespace transport
