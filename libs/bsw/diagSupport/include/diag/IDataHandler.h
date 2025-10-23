// Copyright 2025 Accenture.

#ifndef GUARD_DDC9CAFF_1883_4788_9E4C_F5E12DCE26C1
#define GUARD_DDC9CAFF_1883_4788_9E4C_F5E12DCE26C1

#include <estd/slice.h>

namespace diag
{

template<typename Request>
class IDataHandler
{
public:
    using RequestType = Request;

    enum class Result
    {
        OK,
        BUSY,
        UNKNOWN_ID,
    };

    virtual Result handleRequest(RequestType& request, uint8_t priority) = 0;
};

} // namespace diag

#endif // GUARD_DDC9CAFF_1883_4788_9E4C_F5E12DCE26C1
