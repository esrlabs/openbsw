// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include <transport/TransportMessage.h>

#include <estd/forward_list.h>

namespace transport
{
class ITransportMessageProcessedListener;
}

namespace doip
{
/**
 * DoIP structure for holding a transport message reference together with some admin data.
 */
class DoIpTransportMessageRef : public ::estd::forward_list_node<DoIpTransportMessageRef>
{
public:
    /**
     * Constructor.
     * \param message transport message to send
     * \param notificationListener optional listener to be notified when message has been sent
     * \param refCount initial value of ref counter
     */
    DoIpTransportMessageRef(
        ::transport::TransportMessage& message,
        ::transport::ITransportMessageProcessedListener* notificationListener,
        uint8_t refCount);

    /**
     * Get the represented transport message.
     * \return reference to the message
     */
    ::transport::TransportMessage& getMessage();
    /**
     * Get the optional notification listener.
     * \return pointer to notification listener, may be nullptr
     */
    ::transport::ITransportMessageProcessedListener* getNotificationListener() const;

    /**
     * Set a reference i.e. increase the reference counter
     */
    void setRef();

    /**
     * Get current reference count.
     */
    uint8_t getRef() const;

    /**
     * Release a reference.
     * \return new value of reference counter after decreasing
     */
    uint8_t releaseRef();

private:
    ::transport::TransportMessage& _message;
    ::transport::ITransportMessageProcessedListener* _notificationListener;
    uint8_t _refCount;
};

/**
 * Inline implementations.
 */
inline DoIpTransportMessageRef::DoIpTransportMessageRef(
    ::transport::TransportMessage& message,
    ::transport::ITransportMessageProcessedListener* const notificationListener,
    uint8_t const refCount)
: _message(message), _notificationListener(notificationListener), _refCount(refCount)
{}

inline ::transport::TransportMessage& DoIpTransportMessageRef::getMessage() { return _message; }

inline ::transport::ITransportMessageProcessedListener*
DoIpTransportMessageRef::getNotificationListener() const
{
    return _notificationListener;
}

inline void DoIpTransportMessageRef::setRef() { ++_refCount; }

inline uint8_t DoIpTransportMessageRef::getRef() const { return _refCount; }

inline uint8_t DoIpTransportMessageRef::releaseRef()
{
    --_refCount;
    return _refCount;
}

} // namespace doip
