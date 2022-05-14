/**
 ** Name
 **   led.c
 **
 ** Purpose
 **   LED routines
 **
 ** Revision
 **   27-Aug-2020 (SSB) [] Initial
 **/

#include "led.h"

#define LED_LIVE_CHECK_MS  ((int16_t)1000)
#define LED_LIVE_LED_ON_MS ((int16_t)50)

status_t led_init( void )
{
    status_t         ret             = STATUS_OK;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    LED_GPIO_CLOCK_ENABLE();

    GPIO_InitStruct.Pin   = LED_STATUS_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init( LED_STATUS_PORT, &GPIO_InitStruct );

    HAL_GPIO_WritePin( LED_STATUS_PORT
                     , LED_STATUS_PIN
                     , GPIO_PIN_RESET
                     );

    return ret;
}

void led_set_state( Led_e led, Led_State_e state )
{
    GPIO_PinState new_state = GPIO_PIN_SET;

    if ( LED_ON != state )
    {
        new_state = GPIO_PIN_RESET;
    }

    switch ( led )
    {
        case LED_STATUS:
            HAL_GPIO_WritePin( LED_STATUS_PORT
                             , LED_STATUS_PIN
                             , (GPIO_PinState) new_state
                             );
            break;
        default:
            break;
    }
}

void led_toggle( Led_e led )
{
    switch ( led )
    {
        case LED_STATUS:
            HAL_GPIO_TogglePin( LED_STATUS_PORT, LED_STATUS_PIN );
            break;
        default:
            break;
    }
}

void led_blink_alive( void )
{
    static int16_t    led_time_on = LED_LIVE_LED_ON_MS;
    static int16_t    app_alive   = LED_LIVE_CHECK_MS;

    if ( app_alive > 0 )
    {
        --app_alive;
    }
    else
    {
        if ( led_time_on > 0 )
        {
            --led_time_on;
            led_set_state( LED_STATUS, LED_ON );
        }
        else
        {
            led_set_state( LED_STATUS, LED_OFF );

            app_alive   = LED_LIVE_CHECK_MS;
            led_time_on = LED_LIVE_LED_ON_MS;
        }
    }
}
