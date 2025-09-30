// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/common/IDoIpSendJob.h"

#include <estd/functional.h>

namespace doip
{
/**
 * Abstract class for a send job that consists of exactly 2 buffers:
 * a DoIP header (index = 0) and a single payload buffer (index = 1). For returning the payload
 * buffer a function has to implemented.
 */
class DoIpSimplePayloadSendJob : public IDoIpSendJob
{
public:
    using ReleaseCallbackType = ::estd::function<void(IDoIpSendJob& sendJob, bool success)>;

    /**
     * Constructor.
     * \param protocolVersion DoIP protocol version to put into the header
     * \param payloadType payload type of the DoIP message
     * \param payloadLength length of the payload. This should be the size of the buffer that is
     *        returned from getPayloadBuffer()
     * \param releaseCallback function that will be called to release the send job
     */
    DoIpSimplePayloadSendJob(
        uint8_t protocolVersion,
        uint16_t payloadType,
        uint16_t payloadLength,
        ReleaseCallbackType releaseCallback);

    ~DoIpSimplePayloadSendJob() override{};

    /**
     * Attaches an optional destination endpoint (needed for UDP datagrams) to this send job.
     * \param destinationEndpoint pointer to destination endpoint attached to this job
     */
    void setDestinationEndpoint(::ip::IPEndpoint const* destinationEndpoint);

    /**
     * Sets DoIP message payload type
     * \param payloadType payload type of the DoIP message
     */
    void setPayloadType(uint16_t payloadType);

    uint8_t getSendBufferCount() const override;
    uint16_t getTotalLength() const override;
    ::ip::IPEndpoint const* getDestinationEndpoint() const override;
    void release(bool success) override;
    ::estd::slice<uint8_t const>
    getSendBuffer(::estd::slice<uint8_t> staticBuffer, uint8_t index) override;

protected:
    /**
     * This method returns the payload buffer.
     * \param staticBuffer buffer (>= 8 bytes) that can be used to copy the payload into
     *        The maximum size depends on the configuration of the connection that sends out
     * \return Buffer holding the payload
     */
    virtual ::estd::slice<uint8_t const> getPayloadBuffer(::estd::slice<uint8_t> staticBuffer) const
        = 0;

private:
    enum class BufferIndex : uint8_t
    {
        HEADER,
        PAYLOAD,
        COUNT,
    };

    ReleaseCallbackType _releaseCallback;
    ::ip::IPEndpoint const* _destinationEndpoint;
    uint16_t _payloadLength;
    uint16_t _payloadType;
    uint8_t _protocolVersion;
};

/**
 * Inline implementation.
 */
inline void
DoIpSimplePayloadSendJob::setDestinationEndpoint(::ip::IPEndpoint const* const destinationEndpoint)
{
    _destinationEndpoint = destinationEndpoint;
}

inline void DoIpSimplePayloadSendJob::setPayloadType(uint16_t const payloadType)
{
    _payloadType = payloadType;
}
} // namespace doip
