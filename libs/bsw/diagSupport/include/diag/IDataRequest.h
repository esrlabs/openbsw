// Copyright 2025 Accenture.

#ifndef GUARD_A257464A_9809_403F_9EB6_F8EFE61C3DE6
#define GUARD_A257464A_9809_403F_9EB6_F8EFE61C3DE6

#include <estd/slice.h>

namespace diag
{

class IDataRequest
{
public:
    struct ResponseCode
    {
        static constexpr uint8_t OK = 0U;
        static constexpr uint8_t INACTIVE = 0x22U;
    };

    virtual uint16_t dataId() const                 = 0;
    virtual void sendResponse(uint8_t responseCode) = 0;
};

} // namespace diag

#endif // GUARD_A257464A_9809_403F_9EB6_F8EFE61C3DE6
