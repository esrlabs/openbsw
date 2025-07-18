#pragma once

/**
 * Implementation independent C API for character I/O
 */
#ifdef __cplusplus
extern "C"
{
#endif

using tCharIOApi = struct
{
    void (*init)(void);
    void (*setAsynchronous)(void);
    void (*setSynchronous)(void);
    int (*putc)(int c);
    int (*getc)(void);
    void (*Idle)(void);
    /* functions to be provided by the standard library */
    int (*__outchar)(int c, int last);
    int (*__inchar)(void);
};

/**
 * Function for setting the CharIO API (default = serial CharIO)
 */
void setCharIOApi(tCharIOApi const* api);

/**
 * Reset to the default CharIO (serial)
 * \return
 * - 0 if printing is disabled
 * - nonzero if printing is enabled
 */
int setCharIOApi2Default(void);

/**
 * Make CharIO output asynchronous
 */
void charIoSetAsynchronous(void);

/**
 * Make CharIO output synchronous
 */
void charIoSetSynchronous(void);

/**
 * CharIO idle task for asynchronous output
 */
void charIoBackground(void);

/**
 * CharIO getc
 */
int charIoGetc(void);

/**
 * CharIO putc
 */
int charIoPutc(int c);

// functions below are forwarded to the standard library
int charIO__inchar(void);
int charIO__outchar(int c, int last);

#ifdef __cplusplus
}
#endif
