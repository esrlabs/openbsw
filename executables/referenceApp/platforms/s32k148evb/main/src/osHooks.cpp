// Copyright 2024 Accenture.

#include "FreeRTOS.h"
#include "commonDebug.h"
#include "task.h"

extern "C"
{
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName)
{
    printf("vApplicationStackOverflowHook\r\n");
}

void vIllegalISR()
{
    printf("vIllegalISR\r\n");
    for (;;)
        ;
}

#ifndef BENCHMARK
void FTM4_Ch0_Ch1_IRQHandler() { vIllegalISR(); }
#endif
}
