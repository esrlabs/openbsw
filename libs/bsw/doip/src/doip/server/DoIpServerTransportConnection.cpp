// Copyright 2025 Accenture.

#include "doip/server/DoIpServerTransportConnection.h"

#include "doip/server/DoIpServerTransportConnectionConfig.h"

namespace doip
{
DoIpServerTransportConnection::DoIpServerTransportConnection(
    DoIpConstants::ProtocolVersion const protocolVersion,
    uint8_t const socketGroupId,
    ::tcp::AbstractSocket& socket,
    DoIpServerTransportConnectionConfig const& config,
    ::util::estd::block_pool& diagnosticSendJobBlockPool,
    ::util::estd::block_pool& protocolSendJobBlockPool,
    DoIpTcpConnection::ConnectionType const type)
: DoIpServerConnectionHandler(
    protocolVersion,
    socketGroupId,
    _connection,
    config.getContext(),
    config.getLogicalEntityAddress(),
    config.getParameters())
, ::estd::forward_list_node<DoIpServerTransportConnection>()
, _connection(config.getContext(), socket, _writeBuffer)
, _transportMessageHandler(
      protocolVersion, diagnosticSendJobBlockPool, protocolSendJobBlockPool, config)
, _writeBuffer{}
, _isMarkedForClose(false)
, _type(type)
{
    addMessageHandler(_transportMessageHandler);
}

bool DoIpServerTransportConnection::send(
    ::transport::TransportMessage& transportMessage,
    ::transport::ITransportMessageProcessedListener* const pNotificationListener)
{
    return _transportMessageHandler.send(transportMessage, pNotificationListener);
}

} // namespace doip
