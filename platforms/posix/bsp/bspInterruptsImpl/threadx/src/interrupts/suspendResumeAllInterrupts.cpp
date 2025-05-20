// Copyright 2024 Accenture.

#include "interrupts/suspendResumeAllInterrupts.h"

#include <tx_api.h>
#include <thread>
#include <unistd.h>

static std::thread::id MAIN_THREAD_ID;

void main_thread_setup(void) { MAIN_THREAD_ID = std::this_thread::get_id(); }

OldIntEnabledStatusValueType getOldIntEnabledStatusValueAndSuspendAllInterrupts(void)
{
    return tx_interrupt_control(TX_INT_DISABLE);
}

void resumeAllInterrupts(OldIntEnabledStatusValueType const oldIntEnabledStatusValue)
{
    tx_interrupt_control(oldIntEnabledStatusValue);
}
