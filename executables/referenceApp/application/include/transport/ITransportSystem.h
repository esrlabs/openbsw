// Copyright 2024 Accenture.

#pragma once

#include "transport/ITransportMessageProvider.h"

namespace transport
{
class AbstractTransportLayer;
}

namespace config
{
class ITransportSystem
{
public:
    /** Add a transport layer as a routing target */
    virtual void addTransportLayer(::transport::AbstractTransportLayer& layer)    = 0;
    /** Remove a transport layer as a routing target */
    virtual void removeTransportLayer(::transport::AbstractTransportLayer& layer) = 0;

    virtual ::transport::ITransportMessageProvider& getTransportMessageProvider() = 0;
};

} // namespace config
