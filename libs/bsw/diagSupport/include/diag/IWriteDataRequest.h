// Copyright 2025 Accenture.

#ifndef GUARD_FD01B677_07FE_45D5_902F_EC8CFD59003A
#define GUARD_FD01B677_07FE_45D5_902F_EC8CFD59003A

#include "diag/IDataRequest.h"

#include <estd/slice.h>

namespace diag
{

class IWriteDataRequest : public IDataRequest
{
public:
    virtual ::estd::slice<uint8_t const> getData() const = 0;
};

} // namespace diag

#endif // GUARD_FD01B677_07FE_45D5_902F_EC8CFD59003A
