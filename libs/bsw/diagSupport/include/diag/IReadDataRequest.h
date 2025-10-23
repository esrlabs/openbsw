// Copyright 2025 Accenture.

#ifndef GUARD_DDC9CAFF_1883_4788_9E4C_F5E12DCE26C2
#define GUARD_DDC9CAFF_1883_4788_9E4C_F5E12DCE26C2

#include "diag/IDataRequest.h"

#include <estd/slice.h>

namespace diag
{

class IReadDataRequest : public IDataRequest
{
public:
    virtual void appendData(::estd::slice<uint8_t const> data) = 0;
};

} // namespace diag

#endif // GUARD_DDC9CAFF_1883_4788_9E4C_F5E12DCE26C2
