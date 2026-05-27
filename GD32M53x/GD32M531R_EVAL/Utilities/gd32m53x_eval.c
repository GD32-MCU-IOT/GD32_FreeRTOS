/*!
    \file    gd32m53x_eval.c
    \brief   firmware functions to manage leds, keys, COM ports

    \version 2026-03-04, V1.0.0, demo for GD32M53x
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

#include "gd32m53x_eval.h"

/* private variables */
static uint32_t GPIO_PORT[LEDn]           = {LED1_GPIO_PORT, LED2_GPIO_PORT,
                                             LED3_GPIO_PORT, LED4_GPIO_PORT
                                            };
static uint32_t GPIO_PIN[LEDn]            = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN};

static rcu_periph_enum COM_CLK[COMn]      = {EVAL_COM0_CLK, EVAL_COM1_CLK};
static uint32_t COM_TX_PIN[COMn]          = {EVAL_COM0_TX_PIN, EVAL_COM1_TX_PIN};
static uint32_t COM_RX_PIN[COMn]          = {EVAL_COM0_RX_PIN, EVAL_COM1_RX_PIN};
static uint32_t COM_GPIO_PORT[COMn]       = {EVAL_COM0_GPIO_PORT, EVAL_COM1_GPIO_PORT};
static rcu_periph_enum COM_GPIO_CLK[COMn] = {EVAL_COM0_GPIO_CLK, EVAL_COM1_GPIO_CLK};
static uint32_t COM_GPIO_AF[COMn]         = {EVAL_COM0_AF, EVAL_COM1_AF};

static rcu_periph_enum GPIO_CLK[LEDn]     = {LED1_GPIO_CLK, LED2_GPIO_CLK,
                                             LED3_GPIO_CLK, LED4_GPIO_CLK
                                            };

static uint32_t KEY_PORT[KEYn]            = {WAKEUP_KEY_GPIO_PORT,
                                             USER_KEY_GPIO_PORT
                                            };
static uint32_t KEY_PIN[KEYn]             = {WAKEUP_KEY_PIN,
                                             USER_KEY_PIN
                                            };
static rcu_periph_enum KEY_CLK[KEYn]      = {WAKEUP_KEY_GPIO_CLK,
                                             USER_KEY_GPIO_CLK
                                            };
static exti_line_enum KEY_EXTI_LINE[KEYn] = {WAKEUP_KEY_EXTI_LINE,
                                             USER_KEY_EXTI_LINE
                                            };
static exti_gpio_enum KEY_GPIO_SOURCE[KEYn] = {WAKEUP_KEY_EXTI_GPIO_SOURCE,
                                               USER_KEY_EXTI_GPIO_SOURCE
                                              };
static IRQn_Type KEY_IRQn[KEYn]             = {WAKEUP_KEY_EXTI_IRQn,
                                               USER_KEY_EXTI_IRQn
                                              };

/*!
    \brief      configure led GPIO
    \param[in]  lednum: specify the led to be configured
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void gd_eval_led_init(led_typedef_enum lednum)
{
    /* enable the led clock */
    rcu_periph_clock_enable(GPIO_CLK[lednum]);
    /* configure led GPIO port */
    gpio_mode_set(GPIO_PORT[lednum], GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN[lednum]);
    gpio_output_options_set(GPIO_PORT[lednum], GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, GPIO_PIN[lednum]);

    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      turn on selected led
    \param[in]  lednum: specify the led to be turned on
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void gd_eval_led_on(led_typedef_enum lednum)
{
    GPIO_BOP(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      turn off selected led
    \param[in]  lednum: specify the led to be turned off
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void gd_eval_led_off(led_typedef_enum lednum)
{
    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      toggle selected led
    \param[in]  lednum: specify the led to be toggled
      \arg        LED1
      \arg        LED2
      \arg        LED3
      \arg        LED4
    \param[out] none
    \retval     none
*/
void gd_eval_led_toggle(led_typedef_enum lednum)
{
    gpio_bit_write(GPIO_PORT[lednum], GPIO_PIN[lednum],
                   (bit_status)(1 - gpio_input_bit_get(GPIO_PORT[lednum], GPIO_PIN[lednum])));
}

/*!
    \brief      configure key
    \param[in]  key_num: specify the key to be configured
      \arg        KEY_WAKEUP: wakeup key
      \arg        KEY_USER: user key
    \param[in]  key_mode: specify button mode
      \arg        KEY_MODE_GPIO: key will be used as simple IO
      \arg        KEY_MODE_EXTI: key will be connected to EXTI line with interrupt
    \param[out] none
    \retval     none
*/
void gd_eval_key_init(key_typedef_enum keynum, keymode_typedef_enum keymode)
{
    /* enable the key clock */
    rcu_periph_clock_enable(KEY_CLK[keynum]);
    rcu_periph_clock_enable(RCU_SYSCFG);

    /* configure button pin as input */
    gpio_mode_set(KEY_PORT[keynum], GPIO_MODE_INPUT, GPIO_PUPD_NONE, KEY_PIN[keynum]);

    if(keymode == KEY_MODE_EXTI) {
        /* enable and set key EXTI interrupt to the lowest priority */
        nvic_irq_enable(KEY_IRQn[keynum], 2U, 0U);

        /* connect key EXTI line to key GPIO pin */
        syscfg_exti_line_config(KEY_GPIO_SOURCE[keynum]);

        /* configure key EXTI line */
        exti_init(KEY_EXTI_LINE[keynum], EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(KEY_EXTI_LINE[keynum]);
    }
}

/*!
    \brief      return the selected key state
    \param[in]  key: specify the key to be checked
      \arg        KEY_WAKEUP: wakeup key
      \arg        KEY_USER: user key
    \param[out] none
    \retval     the key's GPIO pin value
*/
uint8_t gd_eval_key_state_get(key_typedef_enum key)
{
    return gpio_input_bit_get(KEY_PORT[key], KEY_PIN[key]);
}

/*!
    \brief      configure COM port
    \param[in]  com: COM on the board
      \arg        EVAL_COM: COM on the board
    \param[out] none
    \retval     none
*/
void gd_eval_com_init(uint32_t com)
{
    uint32_t COM_ID = 0U;

    if(EVAL_COM1 == com){
        COM_ID = 1U;
    }

    /* enable COM GPIO clock */
    rcu_periph_clock_enable(COM_GPIO_CLK[COM_ID]);

    /* enable UART clock */
    rcu_periph_clock_enable(COM_CLK[COM_ID]);

    /* connect port to UARTx_Tx */
    gpio_af_set(COM_GPIO_PORT[COM_ID], COM_GPIO_AF[COM_ID], COM_TX_PIN[COM_ID]);

    /* connect port to UARTx_Rx */
    gpio_af_set(COM_GPIO_PORT[COM_ID], COM_GPIO_AF[COM_ID], COM_RX_PIN[COM_ID]);

    /* configure UART Tx as alternate function push-pull */
    gpio_mode_set(COM_GPIO_PORT[COM_ID], GPIO_MODE_AF, GPIO_PUPD_PULLUP, COM_TX_PIN[COM_ID]);
    gpio_output_options_set(COM_GPIO_PORT[COM_ID], GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, COM_TX_PIN[COM_ID]);

    /* configure UART Rx as alternate function push-pull */
    gpio_mode_set(COM_GPIO_PORT[COM_ID], GPIO_MODE_AF, GPIO_PUPD_PULLUP, COM_RX_PIN[COM_ID]);
    gpio_output_options_set(COM_GPIO_PORT[COM_ID], GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, COM_RX_PIN[COM_ID]);

    /* UART configure */
    uart_deinit(com);
    uart_baudrate_set(com, 115200U);
    uart_receive_config(com, UART_RECEIVE_ENABLE);
    uart_transmit_config(com, UART_TRANSMIT_ENABLE);

    uart_enable(com);
}

#if defined(__GNUC__) && !defined(__clang__) /* For GNU GCC compiler */
/* retarget the C library printf function to the UART, in Eclipse GCC environment */
int __io_putchar(int ch)
{
    uart_data_transmit(EVAL_COM, (uint8_t)ch);
    while(RESET == uart_flag_get(EVAL_COM, UART_FLAG_TBE));
    return ch;
}
#elif (defined (__ICCARM__) && (__VER__ >= 9000000))
#include <LowLevelIOInterface.h>

#pragma module_name = "?__write"

int MyLowLevelPutchar(int x)
{
    uart_data_transmit(EVAL_COM, (uint8_t)x);
    while(RESET == uart_flag_get(EVAL_COM, UART_FLAG_TBE));
    return x;
}

/* retarget the C library printf function to the UART, in IAR __VER__ >= 9000000 environment */
size_t __write(int handle, const unsigned char *buffer, size_t size)
{
    /* Remove the #if #endif pair to enable the implementation */
#if 1
    size_t nChars = 0;

    if(buffer == 0) {
        /*
         * This means that we should flush internal buffers.  Since we
         * don't we just return.  (Remember, "handle" == -1 means that all
         * handles should be flushed.)
         */
        return 0;
    }

    /* This template only writes to "standard out" and "standard err",
     * for all other file handles it returns failure. */
    if(handle != _LLIO_STDOUT && handle != _LLIO_STDERR) {
        return _LLIO_ERROR;
    }

    for(/* Empty */; size != 0; --size) {
        if(MyLowLevelPutchar(*buffer++) < 0) {
            return _LLIO_ERROR;
        }

        ++nChars;
    }

    return nChars;

#else

    /* Always return error code when implementation is disabled. */
    return _LLIO_ERROR;

#endif
}
#else
/* retarget the C library printf function to the UART */
int fputc(int ch, FILE *f)
{
    uart_data_transmit(EVAL_COM, (uint8_t)ch);
    while(RESET == uart_flag_get(EVAL_COM, UART_FLAG_TBE));

    return ch;
}
#endif /* defined(__GNUC__) && !defined(__clang__) */
