/**
 ** Name
 **   gpio.h
 **
 ** Purpose
 **   GPIO routines
 **
 ** Revision
 **   19-Apr-2020 (SSB) [] Initial
 **/

#ifndef __GPIO_H__
#define __GPIO_H__

#include "ptypes.h"

#include <stm32f1xx_hal.h>

#define CLI_PB_PIN  GPIO_PIN_10
#define CLI_PB_PORT GPIOB

#define CLI_PB_GPIO_CLOCK_ENABLE() \
    do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while(0)

status_t gpio_init( void );

#endif /* __GPIO_H__ */
