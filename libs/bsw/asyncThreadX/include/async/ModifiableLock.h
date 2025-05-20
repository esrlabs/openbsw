// Copyright 2021 Accenture.

/**
 * \ingroup async
 */
#ifndef GUARD_5D6895FC_B608_4C8E_87FB_B600671CA5B0
#define GUARD_5D6895FC_B608_4C8E_87FB_B600671CA5B0

#include "tx_api.h"

namespace async
{
class ModifiableLock final
{
public:
    ModifiableLock();
    ~ModifiableLock();

    void unlock();
    void lock();

private:
    unsigned int _key;
    bool _isLocked;
};

/**
 * Inline implementations.
 */
inline ModifiableLock::ModifiableLock() : _key(tx_interrupt_control(TX_INT_DISABLE)), _isLocked(true) {}

inline ModifiableLock::~ModifiableLock()
{
    if (_isLocked)
    {
        tx_interrupt_control(_key);
    }
}

inline void ModifiableLock::unlock()
{
    if (_isLocked)
    {
        tx_interrupt_control(_key);
        _isLocked = false;
    }
}

inline void ModifiableLock::lock()
{
    if (!_isLocked)
    {
        _key      = tx_interrupt_control(TX_INT_DISABLE);
        _isLocked = true;
    }
}

} // namespace async

#endif // GUARD_5D6895FC_B608_4C8E_87FB_B600671CA5B0
