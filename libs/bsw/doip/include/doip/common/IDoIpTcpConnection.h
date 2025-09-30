// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/common/IDoIpConnection.h"

namespace doip
{
/**
 * Interface for a DoIp TCP connection.
 */
class IDoIpTcpConnection : public IDoIpConnection
{
public:
    using DetachCallbackType = ::estd::function<void()>;

    enum class CloseMode : uint8_t
    {
        CLOSE, // TCP FIN is sent when closing
        ABORT  // TCP RST is sent when closing
    };

    /**
     * Set the close mode for this TCP connection.
     * \param closeMode mode to use for closing
     */
    virtual void setCloseMode(CloseMode closeMode) = 0;

    /**
     * Detach from the IP connection without closing or aborting it. If there's a send job
     * that has been sent but has not been acknowledged the connection will wait.
     * \param detachCallback function that will be called when the connection has been
     * detached successfully
     */
    virtual void detach(DetachCallbackType detachCallback) = 0;

protected:
    ~IDoIpTcpConnection() = default;
};

} // namespace doip
