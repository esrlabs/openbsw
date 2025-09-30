// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include <ip/IPEndpoint.h>

#include <estd/forward_list.h>
#include <estd/slice.h>

namespace doip
{
/**
 * Callback interface for a DoIP send job.
 * \tparam T type of send job
 */
template<class T>
class IDoIpSendJobCallback
{
public:
    /**
     * Release the send job.
     * \param sendJob reference to the send job to release
     * \param success true if the send job has been sent successfully
     */
    virtual void releaseSendJob(T& sendJob, bool success) = 0;

protected:
    ~IDoIpSendJobCallback()                                      = default;
    IDoIpSendJobCallback& operator=(IDoIpSendJobCallback const&) = delete;
};

} // namespace doip
