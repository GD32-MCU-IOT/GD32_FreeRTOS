/*
 * FreeRTOS Kernel V11.2.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#if defined(__CC_ARM) || defined(__ICCARM__) || defined(__TASKING__) || defined(__GNUC__)
#include <stdint.h>
#include "gd32w51x_f5hc.h"
extern uint32_t SystemCoreClock;
#endif

/******************************************************************************/
/* Hardware description related definitions. **********************************/
/******************************************************************************/

#define configCPU_CLOCK_HZ                            ( SystemCoreClock )

#define configENABLE_MPU                              0
#define configENABLE_FPU                              1
#define configENABLE_TRUSTZONE                        0

/******************************************************************************/
/* Scheduling behaviour related definitions. **********************************/
/******************************************************************************/

#define configTICK_RATE_HZ                            ( ( TickType_t ) 1000 )
#define configUSE_PREEMPTION                          1
#define configUSE_TIME_SLICING                        1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION       0
#define configUSE_TICKLESS_IDLE                       0   /* Must remain 0 */
#define configMAX_PRIORITIES                          8
#define configMINIMAL_STACK_SIZE                      ( ( uint16_t ) 130 )
#define configMAX_TASK_NAME_LEN                       16
#define configTICK_TYPE_WIDTH_IN_BITS                 TICK_TYPE_WIDTH_32_BITS
#define configIDLE_SHOULD_YIELD                       1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES         1
#define configQUEUE_REGISTRY_SIZE                     8
#define configENABLE_BACKWARD_COMPATIBILITY           1
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS       0

/******************************************************************************/
/* Memory allocation related definitions. *************************************/
/******************************************************************************/

#define configSUPPORT_STATIC_ALLOCATION               0
#define configSUPPORT_DYNAMIC_ALLOCATION              1
#define configTOTAL_HEAP_SIZE                         ( ( size_t ) ( 16 * 1024 ) )

/******************************************************************************/
/* Feature specific definitions. **********************************************/
/******************************************************************************/

#define configUSE_MUTEXES                             1
#define configUSE_RECURSIVE_MUTEXES                   1
#define configUSE_COUNTING_SEMAPHORES                 1
#define configUSE_QUEUE_SETS                          0
#define configUSE_APPLICATION_TASK_TAG                0
#define configUSE_TRACE_FACILITY                      1
#define configUSE_STATS_FORMATTING_FUNCTIONS          0
#define configUSE_CO_ROUTINES                         0
#define configMAX_CO_ROUTINE_PRIORITIES               2

/******************************************************************************/
/* Hook and diagnostic related definitions. ***********************************/
/******************************************************************************/

#define configUSE_IDLE_HOOK                           0
#define configUSE_TICK_HOOK                           0
#define configCHECK_FOR_STACK_OVERFLOW                0
#define configUSE_MALLOC_FAILED_HOOK                  0
#define configUSE_DAEMON_TASK_STARTUP_HOOK            0

#define configGENERATE_RUN_TIME_STATS                 0
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#define portGET_RUN_TIME_COUNTER_VALUE()              0

/* Assert: halt with interrupts disabled on failure.
 * Can be disabled (set to nothing) to reduce code size in production builds. */
#define configASSERT( x )   if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/******************************************************************************/
/* Software timer related definitions. ****************************************/
/******************************************************************************/

#define configUSE_TIMERS                              0
#define configTIMER_TASK_PRIORITY                     2
#define configTIMER_QUEUE_LENGTH                      10
#define configTIMER_TASK_STACK_DEPTH                  ( configMINIMAL_STACK_SIZE * 2 )

/******************************************************************************/
/* Standard library and API inclusion definitions. ****************************/
/******************************************************************************/

#define configUSE_NEWLIB_REENTRANT                    0

#define INCLUDE_vTaskPrioritySet                      1
#define INCLUDE_uxTaskPriorityGet                     1
#define INCLUDE_vTaskDelete                           1
#define INCLUDE_vTaskSuspend                          1
#define INCLUDE_xResumeFromISR                        1
#define INCLUDE_vTaskDelayUntil                       1
#define INCLUDE_vTaskDelay                            1
#define INCLUDE_xTaskGetSchedulerState                1
#define INCLUDE_xTaskGetCurrentTaskHandle             1
#define INCLUDE_uxTaskGetStackHighWaterMark           0
#define INCLUDE_xTaskGetIdleTaskHandle                0
#define INCLUDE_eTaskGetState                         1
#define INCLUDE_xEventGroupSetBitFromISR              1
#define INCLUDE_xTimerPendFunctionCall                0
#define INCLUDE_xTaskAbortDelay                       0
#define INCLUDE_xTaskGetHandle                        0
#define INCLUDE_xTaskResumeFromISR                    1

/******************************************************************************/
/* Cortex-M interrupt priority configuration.                                 */
/******************************************************************************/

#ifdef __NVIC_PRIO_BITS
    /* Use the value defined by the CMSIS device header. */
    #define configPRIO_BITS                           __NVIC_PRIO_BITS
#else
    #define configPRIO_BITS                           4
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
 * function.  With 4 priority bits this is 15 (0x0F).                        */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY       15

/* The highest interrupt priority that can be used by any ISR that calls
 * interrupt-safe FreeRTOS API functions.  DO NOT call such APIs from any
 * interrupt with a HIGHER priority (= lower numeric value) than this!        */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY  2

/* Derived kernel priority values — do not modify directly.                   */
#define configKERNEL_INTERRUPT_PRIORITY               ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY          ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Map FreeRTOS port interrupt handlers to CMSIS/GD32 standard ISR names.
 * These macros must match the handler names in the device startup file.      */
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
