// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include <platform/estdint.h>

namespace doip
{
/**
 * Interface for retrieving information about entity status.
 */
class IDoIpServerEntityStatusCallback
{
public:
    struct EntityStatus
    {
        EntityStatus(
            uint8_t nodeType,
            uint8_t maxConnectionCount,
            uint8_t connectionCount,
            uint32_t maxDataSize);

        //  node type of the entity
        uint8_t const _nodeType;
        //  maximum number of connections allowed for the socket group
        uint8_t const _maxConnectionCount;
        //  number of active connections for the socket group
        uint8_t const _connectionCount;
        //  maximum data size for logical requests
        uint32_t const _maxDataSize;
    };

    /**
     * Get the entity status.
     * \param socketGroupId identifier of socket group to get entity status for
     * \return structure holding the entity status
     */
    virtual EntityStatus getEntityStatus(uint8_t socketGroupId) const = 0;

protected:
    ~IDoIpServerEntityStatusCallback() = default;

    IDoIpServerEntityStatusCallback& operator=(IDoIpServerEntityStatusCallback const&) = default;
};

/**
 * Inline implementations.
 */
inline IDoIpServerEntityStatusCallback::EntityStatus::EntityStatus(
    uint8_t const nodeType,
    uint8_t const maxConnectionCount,
    uint8_t const connectionCount,
    uint32_t const maxDataSize)
: _nodeType(nodeType)
, _maxConnectionCount(maxConnectionCount)
, _connectionCount(connectionCount)
, _maxDataSize(maxDataSize)
{}

} // namespace doip
