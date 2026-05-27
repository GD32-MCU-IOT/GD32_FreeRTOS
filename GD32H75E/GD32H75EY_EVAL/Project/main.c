/*!
    \file    main.c
    \brief   running led

    \version 2026-02-26, V1.1.0, demo for GD32H75E
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

#include "gd32h75e.h"
#include "gd32h75ey_eval.h"
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

void cache_enable(void);
void mpu_config(void);

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
    /* enable the CPU cache */
    cache_enable();
    mpu_config();

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
    gd_eval_com_init(EVAL_COM);

    printf("System booted, init_task started.\r\n");

    gd_eval_led_init(LED3);
    gd_eval_led_on(LED3);
    gd_eval_led_init(LED4);
    gd_eval_led_off(LED4);
    /* create a binary semaphore. */
    binary_semaphore = xSemaphoreCreateBinary();
    /* start toggle LED task every 500ms */
    xTaskCreate(led1_task, "LED3", LED_TASK_STACK, NULL, LED_TASK_PRIO, NULL);
    xTaskCreate(led2_task, "LED4", LED_TASK_STACK, NULL, LED_TASK_PRIO, NULL);

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
        /* toggle LED3 each 500ms */
        printf("led1_task heartbeat %lu\r\n", (unsigned long)heartbeat++);
        gd_eval_led_toggle(LED3);
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
    printf("led2_task created, waiting for key event.\r\n");
    for( ;; ){
        if(NULL != binary_semaphore){
            err = xSemaphoreTake(binary_semaphore, portMAX_DELAY);
            if(pdTRUE == err){
                printf("The key of Wakeup is pressed.\r\n");
                gd_eval_led_toggle(LED4);
            }
        }
        else if(pdFALSE == err){
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
}

/*!
    \brief      enable the CPU cache
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cache_enable(void)
{
    /* enable i-cache */
    SCB_EnableICache();
    /* enable d-cache */
    SCB_EnableDCache();
}

/*!
    \brief      configure the MPU attributes
    \param[in]  none
    \param[out] none
    \retval     none
*/
void mpu_config(void)
{
    mpu_region_init_struct mpu_init_struct;
    mpu_region_struct_para_init(&mpu_init_struct);

    /* disable the MPU */
    ARM_MPU_Disable();
    ARM_MPU_SetRegion(0, 0);

    /* configure the MPU attributes for the entire 4GB area, Reserved, no access */
    /* This configuration is highly recommended to prevent Speculative Prefetching of external memory,
       which may cause CPU read locks and even system errors */
    mpu_init_struct.region_base_address  = 0x0;
    mpu_init_struct.region_size          = MPU_REGION_SIZE_4GB;
    mpu_init_struct.access_permission    = MPU_AP_NO_ACCESS;
    mpu_init_struct.access_bufferable    = MPU_ACCESS_NON_BUFFERABLE;
    mpu_init_struct.access_cacheable     = MPU_ACCESS_NON_CACHEABLE;
    mpu_init_struct.access_shareable     = MPU_ACCESS_SHAREABLE;
    mpu_init_struct.region_number        = MPU_REGION_NUMBER0;
    mpu_init_struct.subregion_disable    = 0x87;
    mpu_init_struct.instruction_exec     = MPU_INSTRUCTION_EXEC_NOT_PERMIT;
    mpu_init_struct.tex_type             = MPU_TEX_TYPE0;
    mpu_region_config(&mpu_init_struct);
    mpu_region_enable();

    /* enable the MPU */
    ARM_MPU_Enable(MPU_MODE_PRIV_DEFAULT);
}
