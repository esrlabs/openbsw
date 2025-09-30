// Copyright 2025 Accenture.

#include "doip/common/DoIpVehicleIdentificationRequestSendJob.h"

#include "doip/common/DoIpConstants.h"

namespace doip
{
DoIpVehicleIdentificationRequestSendJob::DoIpVehicleIdentificationRequestSendJob(
    ReleaseCallbackType const releaseCallback)
: DoIpStaticPayloadSendJob(
    0xffU,
    DoIpConstants::PayloadTypes::VEHICLE_IDENTIFICATION_REQUEST_MESSAGE,
    ::estd::slice<uint8_t const>(),
    releaseCallback)
{}

} // namespace doip
