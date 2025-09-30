// Copyright 2025 Accenture.

#include "doip/common/DoIpStaticPayloadSendJob.h"

namespace doip
{
using ::estd::slice;

DoIpStaticPayloadSendJob::DoIpStaticPayloadSendJob(
    uint8_t const protocolVersion,
    uint16_t const payloadType,
    slice<uint8_t const> const payload,
    ReleaseCallbackType const releaseCallback)
: DoIpSimplePayloadSendJob(
    protocolVersion, payloadType, static_cast<uint16_t>(payload.size()), releaseCallback)
, _payload(payload)
{}

slice<uint8_t const>
DoIpStaticPayloadSendJob::getPayloadBuffer(slice<uint8_t> const /* staticBuffer */) const
{
    return _payload;
}

} // namespace doip
