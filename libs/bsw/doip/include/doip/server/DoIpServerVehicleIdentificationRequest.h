// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include <ip/IPEndpoint.h>

#include <estd/forward_list.h>
#include <estd/variant.h>

namespace doip
{
/**
 * Implements a vehicle identification service.
 */
class DoIpServerVehicleIdentificationRequest
: public ::estd::forward_list_node<DoIpServerVehicleIdentificationRequest>
{
public:
    /**
     * Enumeration defining types of vehicle identification requests.
     */
    enum class ISOType : uint8_t
    {
        IDENTIFICATION,
        ENTITYSTATUS,
        DIAGNOSTICPOWERMODEINFO,
        NACK
    };
    using Type = ::estd::variant<ISOType, uint16_t>;

    /**
     * Constructor.
     * \param destinationEndpoint destination endpoint for response
     * \param type type of identification request
     * \param nackCode NACK code, only valid for TYPE_NACK
     * \param scheduledTimeInMs the timestamp in milliseconds, at which the response will be
     * emitted. It is an absolute timestamp, used as a priority value, to keep the responses
     * ordered.
     */
    DoIpServerVehicleIdentificationRequest(
        ::ip::IPEndpoint const& destinationEndpoint,
        Type const& type,
        uint8_t nackCode,
        uint32_t scheduledTimeInMs);

    /**
     * Get the type of request.
     * \return type
     */
    Type getType() const;

    /**
     * Get the NACK code send (in case of TYPE_NACK)
     */
    uint8_t getNackCode() const;

    /**
     * Get the destination endpoint for the response.
     * \return IP endpoint
     */
    ::ip::IPEndpoint const& getDestinationEndpoint() const;

    /**
     * Get the timestamp in ms when the response must be emitted
     * \return IP endpoint
     */
    uint32_t getScheduledTime() const;

private:
    ::ip::IPEndpoint _destinationEndpoint;
    Type _type;
    uint8_t _nackCode;
    uint32_t const _scheduledTimeInMs = 0; // absolute time
};

/**
 * Implementations.
 */
inline DoIpServerVehicleIdentificationRequest::DoIpServerVehicleIdentificationRequest(
    ::ip::IPEndpoint const& destinationEndpoint,
    Type const& type,
    uint8_t const nackCode,
    uint32_t const scheduledTimeInMs)
: _destinationEndpoint(destinationEndpoint)
, _type(type)
, _nackCode(nackCode)
, _scheduledTimeInMs(scheduledTimeInMs)
{}

inline DoIpServerVehicleIdentificationRequest::Type
DoIpServerVehicleIdentificationRequest::getType() const
{
    return _type;
}

inline uint8_t DoIpServerVehicleIdentificationRequest::getNackCode() const { return _nackCode; }

inline ::ip::IPEndpoint const&
DoIpServerVehicleIdentificationRequest::getDestinationEndpoint() const
{
    return _destinationEndpoint;
}

inline uint32_t DoIpServerVehicleIdentificationRequest::getScheduledTime() const
{
    return _scheduledTimeInMs;
}

} // namespace doip
