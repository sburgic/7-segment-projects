/**
 ** Name
 **   main.c
 **
 ** Purpose
 **   Main application
 **
 ** Revision
 **   07-Dec-2020 (SSB) [] Initial
 **   18-Dec-2021 (SSB) [] Add support for updated 1-Wire library
 **/

#include "main.h"

#include "cli.h"
#include "ds18b20.h"
#include "gpio.h"
#include "led.h"
#include "ptypes.h"
#include "time.h"
#include "tlc59282.h"
#include "uart.h"
#include "system_init.h"

#include <stdio.h>
#include <string.h>

static void critical_error_handler( void )
{
    HAL_Delay( 5000 );
    NVIC_SystemReset();
}

int main( void )
{
    status_t ret;
    Ds_Hdl_t ds;
    uint8_t  ds_out[TLC_NO_OF_SEG + 1] = {0};

    system_clk_cfg();
    HAL_Init();
    bsp_tmr_init();

    bsp_wait( 100, BSP_TIME_MSEC );

    gpio_init();
    led_init();

    ret = uart_init( USART1, 115200 );
    if ( STATUS_OK != ret )
    {
        critical_error_handler();
    }
    else
    {
        printf( "\r\nInfo: Wi-Fi version %d.%d built on %s %s.\r\n"
              , APP_VER_MAJOR
              , APP_VER_MINOR
              , __DATE__
              , __TIME__
              );
        printf( "Info: System Core Clock is %lu MHz.\r\n"
              , SystemCoreClock / 1000000
              );
    }

    ds  = ds18b20_get_hdl();
    ret = ds18b20_init( GPIOA, GPIO_PIN_8 );

    if ( STATUS_OK != ret )
    {
        critical_error_handler();
    }

    GPIO_PinState user_pb_state;

    /* Since PB is manually pressed during the power-on cycle there is no need
     * for full debouncing or usage of interrupt routine to read the state.
     */
    user_pb_state = HAL_GPIO_ReadPin( CLI_PB_PORT, CLI_PB_PIN );

    if ( GPIO_PIN_RESET == user_pb_state )
    {
        /* Do a primitive debouncing */
        bsp_wait( 75, BSP_TIME_MSEC );

        if ( GPIO_PIN_RESET == user_pb_state )
        {
            printf( "Info: Command Line Interface loaded.\r\n\r\n" );
            cli_process();
        }
    }
    else
    {
        ret = tlc59282_init( TLC59282_SPI );
    }

    if ( STATUS_OK != ret )
    {
        critical_error_handler();
    }
    else
    {
        printf( "Info: TLC59282 driver initialized.\r\n" );
        tlc59282_clear();
        tlc59282_led_test();
    }

    for(;;)
    {

        ds18b20_update();

        if ( DS_SENSOR_ERROR != ds->last_temp_raw[0] )
        {
            ds18b20_temp_to_string( ds_out, ds->last_temp_raw[0] );

            if ((( ds->last_temp_raw[0] >> 4 ) > 99 )
             || (( ds->last_temp_raw[0] >> 4 ) < -9 ))
            {
                tlc59282_led_puts( ds_out
                                , strnlen((char*) ds_out, TLC_NO_OF_SEG + 1 )
                                );
            }
            else
            {
                tlc59282_led_puts( ds_out
                                , strnlen((char*) ds_out, TLC_NO_OF_SEG )
                                );
            }
        }
        else
        {
            tlc59282_led_puts((uint8_t*) "Err.", TLC_NO_OF_SEG );
        }

        bsp_wait( 1, BSP_TIME_SEC );
    }

#ifndef __ICCARM__
    return 0;
#endif
}
