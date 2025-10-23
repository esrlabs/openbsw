// Copyright 2025 Accenture.

#ifndef GUARD_BF528E5F_9051_43C9_AD3B_EE6412621035
#define GUARD_BF528E5F_9051_43C9_AD3B_EE6412621035

#include "config/Traits.h"
#include "config/Types.h"

#include <async/Types.h>

#include <estd/array.h>
#include <estd/slice.h>

namespace config
{

namespace internal
{

template<typename Id>
struct Context
{
    using IdType = Id;

    ::async::ContextType context;
};

} // namespace internal

template<typename... CtxIds>
class ContextScope
{
public:
    using ContextIds = Types<CtxIds...>;
    using Ids        = Types<CtxIds...>;
    using InputType  = ::estd::array<::async::ContextType, sizeof...(CtxIds)>;

    struct ContextTableId
    {};

    template<typename Parent, typename... CIds>
    struct Consumer
    {
        template<
            typename CtxId,
            typename = typename ::std::enable_if<
                !::std::is_same<ContextTableId, CtxId>::value
                && Types<CIds...>::template IsContained<CtxId>::value>::type>
        ::async::ContextType getContext() const
        {
            Parent const* parent = static_cast<Parent const*>(this);
            return parent->getScope().template getContext<CtxId>();
        }

        template<
            typename CtxId,
            typename = typename ::std::enable_if<
                ::std::is_same<ContextTableId, CtxId>::value &&
                Types<CIds...>::template IsContained<CtxId>::value>::type>
        ::estd::slice<::async::ContextType const> getContext() const
        {
            return static_cast<Parent const*>(this)->getScope().template getContextTable();
        }
    };

    template<typename Desc>
    static constexpr auto prepareInput(Desc const& desc)
    {
        return InputType{::std::get<::config::internal::Context<CtxIds>>(desc).context...};
    }

    ContextScope(InputType const& contexts) : _contexts(contexts) {}

private:
    template<typename CtxId>
    typename ::async::ContextType getContext() const
    {
        return _contexts.at(Ids::template IndexOf<CtxId>::value);
    }

    ::estd::slice<::async::ContextType const> getContextTable() const
    {
        return ::estd::slice<::async::ContextType const>(_contexts);
    }

    InputType const& _contexts;
};

// free functions
template<typename CtxId>
constexpr auto context(::async::ContextType context)
{
    return ::config::internal::Context<CtxId>{context};
}

} // namespace config

#endif // GUARD_BF528E5F_9051_43C9_AD3B_EE6412621035
