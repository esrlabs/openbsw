// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/server/DoIpServerTransportConnection.h"
#include "doip/server/IDoIpServerTransportConnectionCreator.h"
#include "doip/server/IDoIpServerTransportConnectionPool.h"

#include <estd/object_pool.h>

namespace doip
{
namespace declare
{
template<
    class T,
    size_t NUM_CONNECTIONS,
    size_t NUM_DIAGNOSTICSENDJOBS,
    size_t NUM_PROTOCOLSENDJOBS>
class DoIpServerTransportConnectionPool : public IDoIpServerTransportConnectionPool
{
public:
    /**
     * Number of sockets (and connections) to provide.
     */
    static size_t const NUM_SOCKETS = NUM_CONNECTIONS + 1U;

    /**
     * Constructor.
     * \param creator reference to connection creator interface
     */
    explicit DoIpServerTransportConnectionPool(IDoIpServerTransportConnectionCreator<T>& creator);

    DoIpServerTransportConnection* createConnection(
        uint8_t socketGroupId,
        ::tcp::AbstractSocket& socket,
        DoIpServerTransportConnectionConfig const& config,
        DoIpTcpConnection::ConnectionType type) override;

    void releaseConnection(DoIpServerTransportConnection& connection) override;

private:
    IDoIpServerTransportConnectionCreator<T>& _creator;
    ::estd::declare::object_pool<T, NUM_SOCKETS> _connectionPool;
    ::util::estd::declare::block_pool<
        NUM_DIAGNOSTICSENDJOBS,
        DoIpServerTransportMessageHandler::MIN_DIAGNOSTIC_SENDJOB_SIZE>
        _diagnosticSendJobBlockPool;
    ::util::estd::declare::block_pool<
        NUM_PROTOCOLSENDJOBS,
        DoIpServerTransportMessageHandler::MIN_PROTOCOL_SENDJOB_SIZE>
        _protocolSendJobBlockPool;
};

/**
 * Inline implementations.
 */
template<class T, size_t NUM_SOCKETS, size_t NUM_DIAGNOSTICSENDJOBS, size_t NUM_PROTOCOLSENDJOBS>
DoIpServerTransportConnectionPool<T, NUM_SOCKETS, NUM_DIAGNOSTICSENDJOBS, NUM_PROTOCOLSENDJOBS>::
    DoIpServerTransportConnectionPool(IDoIpServerTransportConnectionCreator<T>& creator)
: IDoIpServerTransportConnectionPool(), _creator(creator)
{}

template<class T, size_t NUM_SOCKETS, size_t NUM_DIAGNOSTICSENDJOBS, size_t NUM_PROTOCOLSENDJOBS>
DoIpServerTransportConnection*
DoIpServerTransportConnectionPool<T, NUM_SOCKETS, NUM_DIAGNOSTICSENDJOBS, NUM_PROTOCOLSENDJOBS>::
    createConnection(
        uint8_t const socketGroupId,
        ::tcp::AbstractSocket& socket,
        DoIpServerTransportConnectionConfig const& config,
        DoIpTcpConnection::ConnectionType const type)
{
    if (!_connectionPool.empty())
    {
        ::estd::constructor<T> constructor = _connectionPool.allocate();
        return &_creator.createConnection(
            constructor,
            socketGroupId,
            socket,
            _diagnosticSendJobBlockPool,
            _protocolSendJobBlockPool,
            config,
            type);
    }
    return nullptr;
}

template<class T, size_t NUM_SOCKETS, size_t NUM_DIAGNOSTICSENDJOBS, size_t NUM_PROTOCOLSENDJOBS>
void DoIpServerTransportConnectionPool<
    T,
    NUM_SOCKETS,
    NUM_DIAGNOSTICSENDJOBS,
    NUM_PROTOCOLSENDJOBS>::releaseConnection(DoIpServerTransportConnection& connection)
{
    // only T can be allocated
    _connectionPool.release(static_cast<T&>(connection));
}

} // namespace declare
} // namespace doip
