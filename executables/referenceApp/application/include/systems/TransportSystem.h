// Copyright 2024 Accenture.

#pragma once

#include <config/ConfigIds.h>
#include <transport/ITransportMessageProvider.h>
#include <transport/ITransportSystem.h>
#include <transport/routing/TransportRouterSimple.h>

namespace config
{
class AbstractTransportLayer;

class TransportSystem
: public ITransportSystem
, public ComponentBase<ScopeType, CtxId<Ctx::DIAG>>
{
public:
    void init();
    void start();
    void stop();

    static constexpr auto services()
    {
        return serviceAccessor<TransportSystem>()
            .service<Id<ITransportSystem>>();
    }

private:
    /** \see ITransportSystem::addTransportLayer() */
    void addTransportLayer(::transport::AbstractTransportLayer& layer) override;
    /** \see ITransportSystem::removeTransportLayer() */
    void removeTransportLayer(::transport::AbstractTransportLayer& layer) override;
    /** \see ITransportSystem::getTransportMessageProvider() */
    ::transport::ITransportMessageProvider& getTransportMessageProvider() override;

    ::transport::TransportRouterSimple _transportRouter;
};

} // namespace config
