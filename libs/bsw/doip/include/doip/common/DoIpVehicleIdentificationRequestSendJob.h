// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/common/DoIpStaticPayloadSendJob.h"

namespace doip
{
/**
 * A simple DoIP client vehicle announcement receiver service.
 * The service broadcasts/multicasts an initial vehicle identification and
 * listens for UDP announcement messages of connected devices.
 */
class DoIpVehicleIdentificationRequestSendJob : public DoIpStaticPayloadSendJob
{
public:
    /**
     * Constructor.
     * \param releaseCallback function that will be called when the send job will be released
     */
    explicit DoIpVehicleIdentificationRequestSendJob(ReleaseCallbackType releaseCallback);
};

} // namespace doip
