// Copyright 2025 Accenture.

#pragma once

#include "ip/NetworkInterfaceConfig.h"

#include <util/estd/signal.h>

#include <estd/functional.h>
#include <estd/ordered_map.h>

namespace ip
{
/**
 * Interface for updating and retrieving IP address configurations of network
 * interfaces.
 *
 * IP addresses are typically assigned dynamically to network interfaces. Therefore components
 * need to get notified about changes of assigned network addresses. This can be done by
 * registering as a listener to config changes.
 */

/**
 * Signal type for listening to configuration changes
 */
using ConfigChangedSignal
    = ::util::estd::signal<::estd::function<void(uint8_t, NetworkInterfaceConfig const&)>>;

/**
 * Update a configuration for a specific network interface identified by its key.
 * \param key key identifying network interface
 * \param config network interface configuration
 */
inline bool updateConfig(NetworkInterfaceConfig& config, NetworkInterfaceConfig const& newConfig)
{
    auto const change = config != newConfig;
    config            = newConfig;
    return change;
}

} // namespace ip
