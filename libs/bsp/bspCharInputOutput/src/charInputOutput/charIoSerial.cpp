#include "charInputOutput/charIoSerial.h"

#include "charInputOutput/CharIOSerialCfg.h"
#include "platform/estdint.h"
#include "sci/SciDevice.h"

#include <stdio.h>

extern "C"
{
namespace // file-local variables moved from global to anonymous namespace
{
char SerialLogger_buffer[CHARIOSERIAL_BUFFERSIZE];
int SerialLogger_bufferInd           = 0;
// use synchronous by default so that less memory is needed
int SerialLogger_consoleAsynchronous = 0;
} // namespace

/**
 * Make logging asynchronous
 */
void SerialLogger_setAsynchronous() { SerialLogger_consoleAsynchronous = 1; }

/**
 * Make logging synchronous
 */
void SerialLogger_setSynchronous() { SerialLogger_consoleAsynchronous = 0; }

/**
 * For checking if logger is initialized or not
 * \return
 * - 1 if already initialized
 * - 0 if not yet initialized
 */
uint8_t SerialLogger_getInitState() { return (sciGetInitState()); }

// below functions documented in the header file
void SerialLogger_init()
{
    // setup UART on ESCIA
    sciInit();
}

int SerialLogger_putc(int const c)
{
    int i = 0;
    if (SerialLogger_getInitState() == 0U)
    {
        SerialLogger_init();
    }
    while (sciGetTxNotReady() != 0U)
    {
        i++;
        if (i > SCI_LOGGERTIMEOUT)
        {
            break;
        }
    }

    sciPuth(c);

    return 1;
}

int SerialLogger_getc()
{
    if (sciGetRxReady() != 0U)
    {
        return (static_cast<int>(sciGeth()));
    }
    else
    {
        return 0;
    }
}

void SerialLogger_Idle()
{
    for (int i = 0; i < SerialLogger_bufferInd; i++)
    {
        (void)SerialLogger_putc(static_cast<int>(SerialLogger_buffer[i]));
    }
    SerialLogger_bufferInd = 0;
}

int SerialLogger__outchar(int const c, int const last)
{
    (void)last;
    if (SerialLogger_consoleAsynchronous == 0)
    {
        // synchronous output
        (void)SerialLogger_putc(c);
        return 0;
    }

    // asynchronous output
    SerialLogger_buffer[SerialLogger_bufferInd] = static_cast<char>(c);
    SerialLogger_bufferInd++;
    if (SerialLogger_bufferInd >= CHARIOSERIAL_BUFFERSIZE)
    {
        SerialLogger_bufferInd = 0;
    }
    return 0;
}

int SerialLogger__inchar()
{
    if (SerialLogger_getInitState() == 0U)
    {
        SerialLogger_init();
    }

    while (sciGetRxReady() != 1U)
    {
        ; // wait for receive data to be available
    }

    return static_cast<int>(sciGeth());
}

} // extern "C"
