// Copyright 2025 Accenture.

#ifndef GUARD_F6E40A8B_B778_4C6A_B3F7_F9BB653812B6
#define GUARD_F6E40A8B_B778_4C6A_B3F7_F9BB653812B6

namespace config
{

template<typename... NestedHandlers>
class AddOnHandler
{
public:
    template<typename Parent, typename Scope, typename Component>
    class For : public NestedHandlers::For<Parent, Scope, Component>...
    {
    private:
    public:
        void postInit() { CallHelper<NestedHandlers...>::postInit(static_cast<Parent&>(*this)); }

        void postStart() { CallHelper<NestedHandlers...>::postStart(static_cast<Parent&>(*this)); }

        void preStop() { CallHelper<NestedHandlers...>::preStop(static_cast<Parent&>(*this)); }

    private:
        template<typename...>
        struct CallHelper
        {
            static void postInit(Parent&) {}

            static void postStart(Parent&) {}

            static void preStop(Parent&) {}
        };

        template<typename H, typename... Hs>
        struct CallHelper<H, Hs...>
        {
            static void postInit(Parent& parent)
            {
                static_cast<typename H::For<Parent, Scope, Component>&>(parent).postInit();
                CallHelper<Hs...>::postInit(parent);
            }

            static void postStart(Parent& parent)
            {
                static_cast<typename H::For<Parent, Scope, Component>&>(parent).postStart();
                CallHelper<Hs...>::postStart(parent);
            }

            static void preStop(Parent& parent)
            {
                static_cast<typename H::For<Parent, Scope, Component>&>(parent).preStop();
                CallHelper<Hs...>::preStop(parent);
            }
        };
    };
};

} // namespace config

#endif // GUARD_F6E40A8B_B778_4C6A_B3F7_F9BB653812B6
