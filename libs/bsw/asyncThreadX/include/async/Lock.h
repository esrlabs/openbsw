// Copyright 2021 Accenture.

/**
 * \ingroup async
 */
#ifndef GUARD_C018654B_ADA3_4244_B77C_1B663003B727
#define GUARD_C018654B_ADA3_4244_B77C_1B663003B727

#include "tx_api.h"

namespace async
{

class Lock
{
public:
    Lock();
    ~Lock();

private:
    unsigned int _key;
};

/**
 * Inline implementations.
 */
// TODO: don't use interrupt locks while ThreadX is not yet initialized
//inline Lock::Lock() : _key(tx_interrupt_control(TX_INT_DISABLE)) {}
inline Lock::Lock() : _key(0) {}

// TODO: don't use interrupt locks while ThreadX is not yet initialized
//inline Lock::~Lock() { tx_interrupt_control(_key); }
inline Lock::~Lock() { }

} // namespace async

#endif // GUARD_C018654B_ADA3_4244_B77C_1B663003B727
