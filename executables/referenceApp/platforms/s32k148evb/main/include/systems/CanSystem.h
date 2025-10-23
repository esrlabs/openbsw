// Copyright 2024 Accenture.

#pragma once

#include "can/transceiver/canflex2/CanFlex2Transceiver.h"

#include <config/ConfigIds.h>

#include <estd/singleton.h>

namespace bios
{
class IEcuPowerStateController;
}

extern "C"
{
//[EXTERN_START]
/**
 * This is a C-style function that handles the CAN receive interrupt service routine (ISR).
 *
 * It enters the ISR group for CAN, locks the async task and calls the receiveInterrupt method on
 * CanFlex2Transceiver. If any frames are received, it dispatches the RX task. Finally, it leaves
 * the ISR group for CAN.
 */
extern void call_can_isr_RX();

/**
 * This is a C-style function that handles the CAN transmit interrupt service routine (ISR).
 *
 * It enters the ISR group for CAN, calls the transmitInterrupt method on CanFlex2Transceiver and
 * then leaves the ISR group for CAN.
 */
extern void call_can_isr_TX();
//[EXTERN_END]
}

class StaticBsp;

namespace config
{

class CanSystem
: public ComponentBase<ScopeType, CtxId<Ctx::CAN>, Types<Id<StaticBsp>>>
, public ::estd::singleton<CanSystem>
{
public:
    CanSystem();
    CanSystem(CanSystem const&)            = delete;
    CanSystem& operator=(CanSystem const&) = delete;

    // [PUBLIC_API_START]
    void init();

    void start();

    void stop();

    static constexpr auto services()
    {
        return serviceAccessor<CanSystem>()
            .member<::bios::CanFlex2Transceiver, &CanSystem::_transceiver0>()
            .service<CanId<Bus::CAN_0>>();
    }

    /**
     * Executes the CanRxRunnable in the given context.
     */
    void dispatchRxTask();

    // [PUBLIC_API_END]

    class CanRxRunnable : public ::async::RunnableType
    {
    public:
        /**
         * [CONSTRUCTOR1_START]
         * \param parent The CanSystem object that owns the CanRxRunnable.
         * [CONSTRUCTOR1_END]
         */
        CanRxRunnable(CanSystem& parent);

        // [PUBLIC1_API_START]
        /**
         * This method will be called when CAN frames are received.
         */
        void execute() override;

        /**
         * Sets the enabled state of the CanRxRunnable.
         *
         * \param enabled The state to set.
         */
        void setEnabled(bool enabled) { _enabled = enabled; }

        // [PUBLIC1_API_END]

    private:
        CanSystem& _parent;
        bool _enabled;
    };

private:
    CanSystem(::async::ContextType const context, StaticBsp& staticBsp);

    ::async::ContextType _context;
    bios::CanFlex2Transceiver _transceiver0;
    CanRxRunnable _canRxRunnable;
};

} // namespace config
