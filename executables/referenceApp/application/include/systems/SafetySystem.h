// Copyright 2024 Accenture.

#include <config/ConfigIds.h>
#include <console/AsyncCommandWrapper.h>

namespace config
{
class SafetySystem
: public ComponentBase<ScopeType, CtxId<Ctx::SAFETY>>
, private ::async::IRunnable
{
public:
    SafetySystem() {}
    SafetySystem(SafetySystem const&)            = delete;
    SafetySystem& operator=(SafetySystem const&) = delete;

    void init();
    void start();
    void stop();
    void cyclic();

private:
    void execute() override;

private:
    ::async::TimeoutType _timeout;
};

} // namespace config
