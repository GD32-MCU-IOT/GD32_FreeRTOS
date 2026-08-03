/*!
    \file    gd32f5xx_it.c
    \brief   interrupt service routines

    \version 2024-12-27, V1.2.0, demo for GD32F5xx
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32f527_it.h"
#include "gd32f527i_eval.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

extern SemaphoreHandle_t binary_semaphore;

#define MULTI_ECC_ERROR_HANDLE(s)    do{}while(1)
#define SINGLE_ECC_ERROR_HANDLE(s)   do{}while(1)

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
    if((SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_ECCME0)) ||
            (SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_ECCME1)) ||
            (SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_ECCME2)) ||
            (SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_ECCME3)) ||
            (SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_ECCME4)) ||
            (SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_ECCME5)) ||
            (SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_ECCME6))) {
        MULTI_ECC_ERROR_HANDLE("SRAM or FLASH multi-bits non-correction ECC error\r\n");
    } else {
        /* if NMI exception occurs, go to infinite loop */
        /* HXTAL clock monitor NMI error */
        while(1) {
        }
    }
}

/*!
    \brief      this function handles SRAM and Flash single bit ECC non-correction exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SYSCFG_SINGLE_BIT_ECC_ER_IRQHandler(void)
{
    if((SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_ECCSE0)) ||
            (SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_ECCSE1)) ||
            (SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_ECCSE2)) ||
            (SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_ECCSE3)) ||
            (SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_ECCSE4)) ||
            (SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_ECCSE5)) ||
            (SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_ECCSE6))) {
        MULTI_ECC_ERROR_HANDLE("SRAM or FLASH single bit ECC error\r\n");
    }
}

/*!
    \brief      this function handles FPU exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void FPU_IRQHandler(void)
{
    /* if FPU exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
    /* if DebugMon exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles EXTI0 interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI0_IRQHandler(void)
{
    if(RESET != exti_interrupt_flag_get(WAKEUP_KEY_EXTI_LINE)){
        exti_interrupt_flag_clear(WAKEUP_KEY_EXTI_LINE);
        if(NULL != binary_semaphore){
            xSemaphoreGiveFromISR(binary_semaphore, NULL);
            portYIELD_FROM_ISR(pdTRUE);
        }
    }
}
