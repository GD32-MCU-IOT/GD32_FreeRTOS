/*!
    \file    main.c
    \brief   running LED

    \version 2026-02-26, V2.6.0, demo for GD32E23x
*/

/*
    Copyright (c) 2026, GigaDevice Semiconductor Inc.

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

#include "gd32e23x.h"
#include "gd32e235c_eval.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* FreeRTOS task priorities and stack sizes. */
#define INIT_TASK_PRIO   ( tskIDLE_PRIORITY + 1 )
#define LED_TASK_PRIO    ( tskIDLE_PRIORITY + 2 )
#define INIT_TASK_STACK  ( configMINIMAL_STACK_SIZE * 2U )
#define LED_TASK_STACK   ( configMINIMAL_STACK_SIZE * 2U )

void led1_task(void * pvParameters);
void led2_task(void * pvParameters);
void init_task(void * pvParameters);

/* Binary semaphore handle definition. */
SemaphoreHandle_t binary_semaphore;

static void uart_raw_puts(const char *s)
{
    while(*s) {
        usart_data_transmit(EVAL_COM, (uint8_t)(*s++));
        while(RESET == usart_flag_get(EVAL_COM, USART_FLAG_TBE)) {
        }
    }
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* init task */
    xTaskCreate(init_task, "INIT", INIT_TASK_STACK, NULL, INIT_TASK_PRIO, NULL);

    /* start scheduler */
    vTaskStartScheduler();

    while (1) {
    }
}
/*!
    \brief      init task
    \param[in]  pvParameters not used
    \param[out] none
    \retval     none
*/
void init_task(void * pvParameters)
{
    gd_eval_key_init(KEY_WAKEUP, KEY_MODE_EXTI);

    gd_eval_com_init(EVAL_COM);
    uart_raw_puts("System booted, init_task started.\r\n");

    gd_eval_led_init(LED1);
    gd_eval_led_on(LED1);
    gd_eval_led_init(LED2);
    gd_eval_led_off(LED2);
    /* create a binary semaphore. */
    binary_semaphore = xSemaphoreCreateBinary();
    /* start toggle LED task every 500ms */
    if(pdPASS != xTaskCreate(led1_task, "LED1", LED_TASK_STACK, NULL, LED_TASK_PRIO, NULL)) {
        uart_raw_puts("led1_task create failed!\r\n");
    }
    if(pdPASS != xTaskCreate(led2_task, "LED2", LED_TASK_STACK, NULL, LED_TASK_PRIO, NULL)) {
        uart_raw_puts("led2_task create failed!\r\n");
    }

    for( ;; ) {
        vTaskDelete(NULL);
    }
}

/*!
    \brief      LED1 task
    \param[in]  pvParameters not used
    \param[out] none
    \retval     none
*/
void led1_task(void * pvParameters)
{
    uart_raw_puts("led1_task started.\r\n");
    for( ;; ) {
        /* toggle LED1 each 500ms */
        uart_raw_puts("led1_task heartbeat\r\n");
        gd_eval_led_toggle(LED1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

/*!
    \brief      LED2 task
    \param[in]  pvParameters not used
    \param[out] none
    \retval     none
*/
void led2_task(void * pvParameters)
{
    BaseType_t err = pdFALSE;

    uart_raw_puts("led2_task created, waiting for key event.\r\n");
    for( ;; ) {
        if(NULL != binary_semaphore) {
            err = xSemaphoreTake(binary_semaphore, portMAX_DELAY);
            if(pdTRUE == err) {
                uart_raw_puts("The key of Wakeup is pressed.\r\n");
                gd_eval_led_toggle(LED2);
            }
        }
        else {
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
}
