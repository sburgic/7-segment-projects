/**
 ** Name
 **   gpio.c
 **
 ** Purpose
 **   GPIO routines
 **
 ** Revision
 **   19-Apr-2020 (SSB) [] Initial
 **/

#include "gpio.h"

status_t gpio_init( void )
{
    status_t         ret             = STATUS_OK;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    CLI_PB_GPIO_CLOCK_ENABLE();

    GPIO_InitStruct.Pin   = CLI_PB_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init( CLI_PB_PORT, &GPIO_InitStruct );

    HAL_GPIO_WritePin( CLI_PB_PORT
                     , CLI_PB_PIN
                     , GPIO_PIN_SET
                     );

    return ret;
}
