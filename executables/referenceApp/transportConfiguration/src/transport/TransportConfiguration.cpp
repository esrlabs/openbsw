// Copyright 2026 Accenture.

#include "transport/TransportConfiguration.h"

namespace transport
{
constexpr TransportConfiguration::TesterAddresses const
    TransportConfiguration::TESTER_ADDRESS_RANGE;

template<>
etl::array<::etl::span<LogicalAddress const>, TransportConfiguration::NUMBER_OF_ADDRESS_LISTS> const
    TransportConfiguration::LogicalAddressConverterGateway::TESTER_ADDRESS_LISTS
    = {TransportConfiguration::TESTER_ADDRESS_RANGE};
} // namespace transport
