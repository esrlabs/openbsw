// Copyright 2025 Accenture.

#ifndef GUARD_F51D5577_0FFC_4A70_9855_4A2A72B16722
#define GUARD_F51D5577_0FFC_4A70_9855_4A2A72B16722

#include "config/Types.h"

namespace config
{

struct IComponentCallback
{
    virtual void transitionDone() = 0;
};

template<typename, typename, typename = Types<>>
class ComponentBase
{};

template<typename Scope, typename DefCtxId, typename... ConsumedIds>
class ComponentBase<Scope, DefCtxId, Types<ConsumedIds...>>
: public Scope::Consumer<DefCtxId, ConsumedIds...>
{
public:
    using ScopeType    = Scope;
    using DefaultCtxId = DefCtxId;

    void initCallback(IComponentCallback& callback) { _callback = &callback; }

protected:
    void transitionDone()
    {
        if (_callback != nullptr)
        {
            _callback->transitionDone();
        }
    }

private:
    IComponentCallback* _callback = nullptr;
};

} // namespace config

#endif // GUARD_F51D5577_0FFC_4A70_9855_4A2A72B16722
