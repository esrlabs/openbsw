// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/server/IDoIpServerVehicleIdentificationCallback.h"
#include "doip/server/IDoIpUdpOemMessageHandler.h"

#include <estd/functional.h>
#include <estd/ordered_map.h>

namespace doip
{
/**
 * Callback based implementation for accessing vehicle identification data.
 */
class DoIpServerVehicleIdentification : public IDoIpServerVehicleIdentificationCallback
{
public:
    /** Fills a given slice with the vehicle's VIN. */
    using GetVinCallback = ::estd::function<void(VinType)>;

    /** Fills a given slice with the DoIP group ID. */
    using GetGidCallback = ::estd::function<void(GidType)>;

    /** Fills a given slice with the DoIP entity ID. */
    using GetEidCallback = ::estd::function<void(EidType)>;

    /** Returns current diagnostic power mode. */
    using GetPowerModeCallback = ::estd::function<DoIpConstants::DiagnosticPowerMode()>;

    /** Callback whenever a Vehicle Identification Request has been received. */
    using OnVirReceivedCallback = ::estd::function<void()>;

    /**
     * Constructor.
     * \param getVinCallback callback function that will be called to retrieve the VIN
     * \param getGidCallback callback function that will be called to retrieve the GID
     * \param getEidCallback callback function that will be called to retrieve the EID
     * \param getPowerModeCallback callback function that will be called to retrieve the power mode
     * \param virReceivedCallback callback function that will be called when a Vehicle
     * Identification Request has been received \param oemMessageHandlers dispatch map for handling
     * OEM custom messages. Parameter is nullable, if user is not interested in handling OEM
     * extensions to ISO
     */
    DoIpServerVehicleIdentification(
        GetVinCallback getVinCallback,
        GetGidCallback getGidCallback,
        GetEidCallback getEidCallback,
        GetPowerModeCallback getPowerModeCallback,
        OnVirReceivedCallback onVirReceivedCallback,
        ::estd::ordered_map<uint16_t, IDoIpUdpOemMessageHandler*> const* const oemMessageHandlers);

    void getVin(VinType vin) override;
    void getGid(GidType gid) override;
    void getEid(EidType eid) override;
    DoIpConstants::DiagnosticPowerMode getPowerMode() override;
    void onVirReceived() override;
    IDoIpUdpOemMessageHandler* getOemMessageHandler(uint16_t payloadType) const override;

private:
    GetVinCallback _getVinCallback;
    GetGidCallback _getGidCallback;
    GetEidCallback _getEidCallback;
    GetPowerModeCallback _getPowerModeCallback;
    OnVirReceivedCallback _virReceivedCallback;
    ::estd::ordered_map<uint16_t, IDoIpUdpOemMessageHandler*> const* const _oemMessageHandlers;
};

} // namespace doip
