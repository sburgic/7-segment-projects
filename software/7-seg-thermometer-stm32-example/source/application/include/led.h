/**
 ** Name
 **   led.h
 **
 ** Purpose
 **   LED routines
 **
 ** Revision
 **   27-Aug-2020 (SSB) [] Initial
 **/

#ifndef __LED_H__
#define __LED_H__

#include "ptypes.h"

#include <stm32f1xx_hal.h>

#define LED_STATUS_PIN  GPIO_PIN_11
#define LED_STATUS_PORT GPIOA

#define LED_GPIO_CLOCK_ENABLE() \
    do { __HAL_RCC_GPIOA_CLK_ENABLE(); } while(0)

typedef enum
{
    LED_OFF = 0,
    LED_ON
} Led_State_e;

typedef enum
{
    LED_STATUS = 1,
} Led_e;

status_t led_init( void );
void led_set_state( Led_e led, Led_State_e state );
void led_toggle( Led_e led );
void led_blink_alive( void );

#endif /* __LED_H__ */
