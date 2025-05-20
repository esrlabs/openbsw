// Copyright 2021 Accenture.

/**
 * \ingroup async
 */
#ifndef GUARD_18CB5B80_EE86_4FBF_BAD8_015290A7F09C
#define GUARD_18CB5B80_EE86_4FBF_BAD8_015290A7F09C

#include "tx_api.h"

#include <async/Types.h>
#include <util/concurrent/IFutureSupport.h>
#include <estd/string.h>

namespace async
{
class FutureSupport : public ::os::IFutureSupport
{
public:
    explicit FutureSupport(ContextType context);

    void wait() override;
    void notify() override;
    void assertTaskContext() override;
    bool verifyTaskContext() override;

private:
    ContextType _context;
    TX_EVENT_FLAGS_GROUP _eventObject;
    estd::declare::string<10> _eventName;
};

} // namespace async

#endif // GUARD_18CB5B80_EE86_4FBF_BAD8_015290A7F09C
