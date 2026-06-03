/*!
    \file    main.c
    \brief   GPIO running led

    \version 2026-02-12, V3.3.3, demo for GD32F30x
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

#include "gd32f30x.h"
#include "gd32f307c_eval.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>


#define INIT_TASK_PRIO   ( tskIDLE_PRIORITY + 1 )
#define LED_TASK_PRIO    ( tskIDLE_PRIORITY + 2 )
#define INIT_TASK_STACK  ( configMINIMAL_STACK_SIZE * 4U )
#define LED_TASK_STACK   ( configMINIMAL_STACK_SIZE * 4U )

void led1_task(void * pvParameters);
void led2_task(void * pvParameters);
void init_task(void * pvParameters);

/* Binary semaphore handle definition. */
SemaphoreHandle_t binary_semaphore;

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* configure 4 bits pre-emption priority */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);

    /* init task */
    xTaskCreate(init_task, "INIT", INIT_TASK_STACK, NULL, INIT_TASK_PRIO, NULL);

    /* start scheduler */
    vTaskStartScheduler();

    while (1){
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
    gd_eval_com_init(EVAL_COM0);

    printf("System booted, init_task started.\r\n");

    gd_eval_led_init(LED2);
    gd_eval_led_on(LED2);
    gd_eval_led_init(LED3);
    gd_eval_led_off(LED3);
    /* create a binary semaphore. */
    binary_semaphore = xSemaphoreCreateBinary();
    /* start toggle LED tasks */
    xTaskCreate(led1_task, "LED2", LED_TASK_STACK, NULL, LED_TASK_PRIO, NULL);
    xTaskCreate(led2_task, "LED3", LED_TASK_STACK, NULL, LED_TASK_PRIO, NULL);

    for( ;; ){
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
    uint32_t heartbeat = 0U;

    printf("led1_task started.\r\n");
    for( ;; ){
        /* toggle LED2 each 500ms */
        printf("led1_task heartbeat %lu\r\n", (unsigned long)heartbeat++);
        gd_eval_led_toggle(LED2);
        vTaskDelay(500 / portTICK_RATE_MS);
    }
}

/*!
    \brief      LED2 task: waits for WAKEUP key semaphore then toggles LED3
    \param[in]  pvParameters not used
    \param[out] none
    \retval     none
*/
void led2_task(void * pvParameters)
{
    BaseType_t err = pdFALSE;
    for( ;; ){
        if(NULL != binary_semaphore){
            err = xSemaphoreTake(binary_semaphore, portMAX_DELAY);
            if(pdTRUE == err){
                printf("The key of Wakeup is pressed.\r\n");
                gd_eval_led_toggle(LED3);
            }
        }
        else if(pdFALSE == err){
            vTaskDelay(10 / portTICK_RATE_MS);
        }
    }
}
