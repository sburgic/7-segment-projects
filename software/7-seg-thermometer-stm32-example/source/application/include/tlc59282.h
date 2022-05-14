/**
 ** Name
 **   tlc59282.h
 **
 ** Purpose
 **   TLC59282 driver
 **
 ** Revision
 **   07-Dec-2020 (SSB) [] Initial
 **/

#ifndef __TLC59282_H__
#define __TLC59282_H__

#include "ptypes.h"

#include <stm32f1xx_hal.h>

/* Number of connected segments */
#define TLC_NO_OF_SEG ((uint8_t)4)

#define TLC59282_SPI SPI2

/* GPIO mapping */
#define TLC59282_LATCH_PIN  GPIO_PIN_12
#define TLC59282_LATCH_PORT GPIOB

#define TLC59282_LATCH_CLK_ENABLE() \
                do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while(0)

/* Initialize TLC59282 */
status_t tlc59282_init( SPI_TypeDef* spi );
/* Send uint32_t data */
status_t tlc59282_send_uint32( uint32_t data );
/* Send uint32_t data without latch */
status_t tlc59282_send_uint32_nl( uint32_t data );
/* Send uint16_t data */
status_t tlc59282_send_uint16( uint16_t data );
/* Test all segments */
status_t tlc59282_led_test( void );
/* Send string */
status_t tlc59282_led_puts( uint8_t* str, uint8_t size );
/* Clear display - all off */
status_t tlc59282_clear( void );

#endif /* __TLC59282_H__ */
