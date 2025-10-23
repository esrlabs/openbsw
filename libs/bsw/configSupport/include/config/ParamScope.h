// Copyright 2025 Accenture.

#ifndef GUARD_D7550D16_1961_4134_89A1_994FF428B545
#define GUARD_D7550D16_1961_4134_89A1_994FF428B545

#include "config/Traits.h"
#include "config/Types.h"

namespace config
{

namespace internal
{

template<typename Id>
struct Value
{
    using IdType = Id;

    typename Id::ValueType value;
};

} // namespace internal

template<typename... PIds>
class ParamScope
{
public:
    using Ids            = Types<PIds...>;
    using ParamTupleType = ::std::tuple<::config::internal::Value<PIds>...>;
    using InputType      = ParamTupleType;

    template<typename Desc>
    static constexpr auto prepareInput(Desc& desc) -> ParamTupleType
    {
        return ParamTupleType{::std::get<::config::internal::Value<PIds>>(desc)...};
    }

    template<typename Parent, typename... CIds>
    struct Consumer
    {
        template<
            typename PId,
            typename
            = typename ::std::enable_if<Types<CIds...>::template IsContained<PId>::value>::type>
        typename PId::ValueType getParam() const
        {
            return static_cast<Parent const*>(this)->getScope().template getParam<PId>();
        }
    };

    ParamScope(ParamTupleType const& params) : _params(params) {}

private:
    template<typename PId>
    typename PId::ValueType getParam() const
    {
        return ::std::get<Ids::template IndexOf<PId>::value>(_params).value;
    }

    ParamTupleType const& _params;
};

// free functions
template<typename PId, typename Value>
constexpr auto param(Value value)
{
    return ::config::internal::Value<PId>{static_cast<typename PId::ValueType>(value)};
}

} // namespace config

#endif // GUARD_D7550D16_1961_4134_89A1_994FF428B545
