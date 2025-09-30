// Copyright 2025 Accenture.

#include "doip/common/DoIpSimplePayloadSendJob.h"

#include "doip/common/DoIpHeader.h"
#include "doip/common/DoIpSendJobHelper.h"

namespace doip
{
DoIpSimplePayloadSendJob::DoIpSimplePayloadSendJob(
    uint8_t const protocolVersion,
    uint16_t const payloadType,
    uint16_t const payloadLength,
    ReleaseCallbackType const releaseCallback)
: _releaseCallback(releaseCallback)
, _destinationEndpoint(nullptr)
, _payloadLength(payloadLength)
, _payloadType(payloadType)
, _protocolVersion(protocolVersion)
{}

uint8_t DoIpSimplePayloadSendJob::getSendBufferCount() const
{
    return static_cast<uint8_t>(BufferIndex::COUNT);
}

uint16_t DoIpSimplePayloadSendJob::getTotalLength() const
{
    return _payloadLength + static_cast<uint16_t>(DoIpConstants::DOIP_HEADER_LENGTH);
}

::ip::IPEndpoint const* DoIpSimplePayloadSendJob::getDestinationEndpoint() const
{
    return _destinationEndpoint;
}

void DoIpSimplePayloadSendJob::release(bool const success) { _releaseCallback(*this, success); }

::estd::slice<uint8_t const> DoIpSimplePayloadSendJob::getSendBuffer(
    ::estd::slice<uint8_t> const staticBuffer, uint8_t const index)
{
    switch (static_cast<BufferIndex>(index))
    {
        case BufferIndex::HEADER:
        {
            return DoIpSendJobHelper::prepareHeaderBuffer(
                staticBuffer,
                _protocolVersion,
                _payloadType,
                static_cast<uint32_t>(_payloadLength));
        }
        case BufferIndex::PAYLOAD:
        {
            return getPayloadBuffer(staticBuffer);
        }
        default:
        {
            return ::estd::slice<uint8_t const>();
        }
    }
}

} // namespace doip
