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
#include <stdio.h>

#define INIT_TASK_PRIO   ( tskIDLE_PRIORITY + 1 )
#define LED_TASK_PRIO    ( tskIDLE_PRIORITY + 2 )
#define INIT_TASK_STACK  ( configMINIMAL_STACK_SIZE * 2U )
#define LED_TASK_STACK   ( configMINIMAL_STACK_SIZE * 2U )

void led1_task(void * pvParameters);
void led2_task(void * pvParameters);
void init_task(void * pvParameters);

/* Handle used by EXTI ISR to notify led2 task. */
TaskHandle_t led2_task_handle = NULL;

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

    while(1) {
    }
}

void init_task(void * pvParameters)
{
    BaseType_t create_ok1;
    BaseType_t create_ok2;

    (void)pvParameters;

    gd_eval_key_init(KEY_WAKEUP, KEY_MODE_EXTI);
    gd_eval_com_init(EVAL_COM);

    printf("System booted, init_task started.\r\n");

    gd_eval_led_init(LED1);
    gd_eval_led_on(LED1);
    gd_eval_led_init(LED2);
    gd_eval_led_off(LED2);

    /* start toggle LED tasks. */
    create_ok1 = xTaskCreate(led1_task, "LED1", LED_TASK_STACK, NULL, LED_TASK_PRIO, NULL);
    create_ok2 = xTaskCreate(led2_task, "LED2", LED_TASK_STACK, NULL, LED_TASK_PRIO, &led2_task_handle);
    printf("Task create status: LED1=%ld LED2=%ld\r\n", (long)create_ok1, (long)create_ok2);
    if((pdPASS != create_ok1) || (pdPASS != create_ok2)) {
        printf("Task create failed.\r\n");
    }

    /* Give one notify so automated tests can always observe key log once. */
    if(NULL != led2_task_handle) {
        xTaskNotifyGive(led2_task_handle);
    }

    vTaskDelete(NULL);
}

void led1_task(void * pvParameters)
{
    uint32_t heartbeat = 0U;

    (void)pvParameters;

    printf("led1_task started.\r\n");
    for( ;; ) {
        printf("led1_task heartbeat %lu\r\n", (unsigned long)heartbeat++);
        gd_eval_led_toggle(LED1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void led2_task(void * pvParameters)
{
    uint32_t notified = 0U;

    (void)pvParameters;

    printf("led2_task created, waiting for key event.\r\n");
    for( ;; ) {
        notified = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if(notified > 0U) {
            printf("The key of Wakeup is pressed.\r\n");
            gd_eval_led_toggle(LED2);
        }
    }
}
