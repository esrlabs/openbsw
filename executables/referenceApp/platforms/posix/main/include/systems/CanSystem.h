// Copyright 2024 Accenture.

#ifndef GUARD_AD9A8EB2_63B6_42FD_9A69_4F026FD406D0
#define GUARD_AD9A8EB2_63B6_42FD_9A69_4F026FD406D0

#include <can/SocketCanTransceiver.h>
#include <lifecycle/AsyncLifecycleComponent.h>
#include <systems/ICanSystem.h>

namespace systems
{

class CanSystem final
: public ::can::ICanSystem
, public ::lifecycle::AsyncLifecycleComponent
, private ::async::IRunnable
{
public:
    // [PUBLIC_API_START]
    explicit CanSystem(::async::ContextType context);
    CanSystem(CanSystem const&)            = delete;
    CanSystem& operator=(CanSystem const&) = delete;

    ::can::ICanTransceiver* getCanTransceiver(uint8_t busId) override;

    void init() final;
    void run() final;
    void shutdown() final;
    // [PUBLIC_API_END]
private:
    void execute() final;

private:
    ::async::TimeoutType _timeout;
    ::async::ContextType _context;

    ::can::SocketCanTransceiver _canTransceiver;
};

} // namespace systems

#endif // GUARD_AD9A8EB2_63B6_42FD_9A69_4F026FD406D0