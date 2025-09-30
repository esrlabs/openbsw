// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include <estd/constructor.h>

namespace doip
{
/**
 * Interface for creation of a potentially derived transport connection.
 * \tparam T class for transport connection instances, derived from DoIpServerTransportConnection
 */
template<class T>
class IDoIpServerTransportConnectionCreator
{
public:
    /**
     * Create a transport connection instance within already allocated memory.
     * \param constructor reference to constructor
     * \param socketGroupId identifier of socket group the connection belongs to
     * \param socket reference to socket to use
     * \param diagnosticSendJobBlockPool reference to block pool for diagnostic send jobs
     * \param protocolSendJobBlockPool reference to block pool for other protocol send jobs
     * \param config reference to connection configuration
     */
    virtual DoIpServerTransportConnection& createConnection(
        ::estd::constructor<T>& constructor,
        uint8_t socketGroupId,
        ::tcp::AbstractSocket& socket,
        ::util::estd::block_pool& diagnosticSendJobBlockPool,
        ::util::estd::block_pool& protocolSendJobBlockPool,
        DoIpServerTransportConnectionConfig const& config,
        DoIpTcpConnection::ConnectionType type)
        = 0;

protected:
    ~IDoIpServerTransportConnectionCreator() = default;
    IDoIpServerTransportConnectionCreator& operator=(IDoIpServerTransportConnectionCreator const&)
        = default;
};

} // namespace doip
