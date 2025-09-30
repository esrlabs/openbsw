// Copyright 2025 Accenture.

#pragma once

namespace doip
{
/**
 * Callback interface to be notified about opened and closed DoIP connections.
 */
class IDoIpServerConnectionStateCallback
{
public:
    /**
     * Called if a DoIP connection is opened.
     * \param sourceAddress source address of the external tester
     */
    virtual void
    connectionRoutingActive(uint16_t sourceAddress, DoIpTcpConnection::ConnectionType type)
        = 0;
    /**
     * Called if a DoIP connection is closed.
     * \param sourceAddress source address of the external tester
     */
    virtual void connectionClosed(uint16_t sourceAddress) = 0;
};

} // namespace doip
