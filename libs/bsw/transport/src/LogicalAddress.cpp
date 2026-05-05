// Copyright 2024 Accenture.

#include "transport/LogicalAddress.h"

#include <etl/algorithm.h>

namespace transport
{
namespace addressfinder
{
::etl::optional<LogicalAddress>
find2ByteAddressInSlice(uint16_t const address, ::etl::span<LogicalAddress const> const& list)
{
    auto* const iter = etl::find_if(
        list.begin(),
        list.end(),
        [address](LogicalAddress const addr) -> bool { return addr.address2Byte == address; });
    if (iter != list.end())
    {
        return *iter;
    }
    return {};
}

::etl::optional<LogicalAddress>
find1ByteAddressInSlice(uint16_t const address, ::etl::span<LogicalAddress const> const& list)
{
    auto* const iter = etl::find_if(
        list.begin(),
        list.end(),
        [address](LogicalAddress const addr) -> bool { return addr.address1Byte == address; });
    if (iter != list.end())
    {
        return *iter;
    }
    return {};
}
} // namespace addressfinder
} // namespace transport
