/**
 ** Name
 **   tlc59282.c
 **
 ** Purpose
 **   TLC59282 driver
 **
 ** Revision
 **   07-Dec-2020 (SSB) [] Initial
 **/

#include "tlc59282.h"

#include "time.h"

#define TLC59282_SPI_TIMEOUT ((uint16_t)1)

static SPI_HandleTypeDef spi_hdl;

#define SEG_A 0x01
#define SEG_B 0x02
#define SEG_C 0x04
#define SEG_D 0x08
#define SEG_E 0x10
#define SEG_F 0x20
#define SEG_G 0x40
#define SEG_H 0x80

/* ASCII lookup table */
static const uint8_t tlc59282_ascii_tbl[256] =
{
    [32] = 0x00,  /* Space */

    [45] = SEG_G, /* Minus */

    [48]  = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,          /* 0 */
    [49]  = SEG_B | SEG_C,                                          /* 1 */
    [50]  = SEG_A | SEG_B | SEG_G | SEG_D | SEG_E,                  /* 2 */
    [51]  = SEG_A | SEG_B | SEG_C | SEG_D | SEG_G,                  /* 3 */
    [52]  = SEG_F | SEG_G | SEG_B | SEG_C,                          /* 4 */
    [53]  = SEG_A | SEG_F | SEG_C | SEG_D | SEG_G,                  /* 5 */
    [54]  = SEG_A | SEG_F | SEG_C | SEG_D | SEG_G | SEG_E,          /* 6 */
    [55]  = SEG_A | SEG_B | SEG_C,                                  /* 7 */
    [56]  = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,  /* 8 */
    [57]  = SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G,          /* 9 */

    [58]  = SEG_H,                                                  /* : */

    [69]  = SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,                  /* E */
    [83]  = SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,                  /* S */
    [99]  = SEG_D | SEG_E | SEG_G,                                  /* c */
    [100] = SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,                  /* d */
    [114] = SEG_E | SEG_G,                                          /* r */
    [117] = SEG_C | SEG_D | SEG_E,                                  /* u */

};

static __inline__ void tlc59282_latch( void )
{
    HAL_GPIO_WritePin ( TLC59282_LATCH_PORT
                      , TLC59282_LATCH_PIN
                      , GPIO_PIN_SET
                      );

    bsp_wait( 1, BSP_TIME_USEC );

    HAL_GPIO_WritePin ( TLC59282_LATCH_PORT
                      , TLC59282_LATCH_PIN
                      , GPIO_PIN_RESET
                      );
}

static void tlc59282_gpio_init( void )
{
    GPIO_InitTypeDef gpio = {0};

    TLC59282_LATCH_CLK_ENABLE();

    gpio.Pin   = TLC59282_LATCH_PIN;
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init( TLC59282_LATCH_PORT, &gpio );

    HAL_GPIO_WritePin( TLC59282_LATCH_PORT
                     , TLC59282_LATCH_PIN
                     , GPIO_PIN_RESET
                     );
}

static status_t tlc59282_send_byte( uint8_t byte )
{
    status_t          ret = STATUS_OK;
    HAL_StatusTypeDef hret;

    hret = HAL_SPI_Transmit( &spi_hdl, &byte, 1, TLC59282_SPI_TIMEOUT );

    if ( HAL_OK != hret )
    {
        ret = STATUS_ERROR;
    }

    return ret;
}

status_t tlc59282_init( SPI_TypeDef* spi )
{
    status_t          ret = STATUS_OK;
    HAL_StatusTypeDef hret;

    spi_hdl.Instance               = spi;
    spi_hdl.Init.Mode              = SPI_MODE_MASTER;
    spi_hdl.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    spi_hdl.Init.Direction         = SPI_DIRECTION_2LINES;
    spi_hdl.Init.CLKPhase          = SPI_PHASE_1EDGE;
    spi_hdl.Init.CLKPolarity       = SPI_POLARITY_LOW;
    spi_hdl.Init.DataSize          = SPI_DATASIZE_8BIT;
    spi_hdl.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    spi_hdl.Init.TIMode            = SPI_TIMODE_DISABLE;
    spi_hdl.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    spi_hdl.Init.CRCPolynomial     = 7;
    spi_hdl.Init.NSS               = SPI_NSS_SOFT;

    hret = HAL_SPI_Init( &spi_hdl );

    if ( HAL_OK != hret )
    {
        ret = STATUS_ERROR;
    }
    else
    {
        tlc59282_gpio_init();
    }

    return ret;
}

status_t tlc59282_send_uint32( uint32_t data )
{
    status_t ret = STATUS_OK;
    uint8_t  array[4];
    uint8_t  i;

    array[3] = ( data & 0xFF );
    array[2] = ((data >> 8) & 0xFF );
    array[1] = ((data >> 16) & 0xFF );
    array[0] = ((data >> 24) & 0xFF );

    for ( i = 0; i < 4; i++ )
    {
        ret |= tlc59282_send_byte( array[i] );
    }

    if ( STATUS_OK == ret )
    {
        tlc59282_latch ();
    }

    return ret;
}

status_t tlc59282_send_uint32_nl( uint32_t data )
{
    status_t ret = STATUS_OK;
    uint8_t  array[4];
    uint8_t  i;

    array[3] = ( data & 0xFF );
    array[2] = ((data >> 8) & 0xFF );
    array[1] = ((data >> 16) & 0xFF );
    array[0] = ((data >> 24) & 0xFF );

    for ( i = 0; i < 4; i++ )
    {
        ret |= tlc59282_send_byte ( array[i] );
    }

    return ret;
}

status_t tlc59282_send_uint16( uint16_t data )
{
    status_t ret = STATUS_OK;
    uint8_t  array[2];
    uint8_t  i;

    array[1] = ( data & 0xFF );
    array[0] = ((data >> 8) & 0xFF );

    for ( i = 0; i < 2; i++ )
    {
        ret |= tlc59282_send_byte( array[i] );
    }

    if ( STATUS_ERROR != ret )
    {
        tlc59282_latch();
    }

    return ret;
}

status_t tlc59282_led_puts( uint8_t* str, uint8_t size )
{
    status_t ret = STATUS_OK;
    uint8_t  i;
    uint8_t  byte;
    uint8_t  skipped = 0;

    if ( NULL != str )
    {
        for ( i = 0; i < size; i++ )
        {
            byte = (uint8_t) str[i];

            if (( i < ( size - 1 )) && ( '.' == str[i + 1]))
            {
                ret |= tlc59282_send_byte( tlc59282_ascii_tbl[byte] | SEG_H );
            }
            else if ( '.' != byte )
            {
                ret |= tlc59282_send_byte( tlc59282_ascii_tbl[byte] );
            }
            else
            {
                skipped++;
            }
        }

        /* Clear till end */
        for ( i = ( size - skipped ); i < TLC_NO_OF_SEG; i++ )
        {
            ret |= tlc59282_send_byte( tlc59282_ascii_tbl[32] ); /* Space */
        }

        if ( STATUS_OK == ret )
        {
            tlc59282_latch();
        }
    }
    else
    {
        ret = STATUS_ERROR;
    }

    return ret;
}

status_t tlc59282_led_test( void )
{
    status_t ret;

    ret  = tlc59282_send_uint32_nl  (   SEG_A
                                    | ( SEG_A << 8)
                                    | ( SEG_A << 16 )
                                    | ( SEG_A << 24 )
                                    );
    ret |= tlc59282_send_uint16     (   SEG_A
                                    | ( SEG_A << 8)
                                    );
    bsp_wait( 500, BSP_TIME_MSEC );

    ret |= tlc59282_send_uint32_nl  (   SEG_B
                                    | ( SEG_B << 8)
                                    | ( SEG_B << 16 )
                                    | ( SEG_B << 24 )
                                    );
    ret |= tlc59282_send_uint16     (   SEG_B
                                    | ( SEG_B << 8)
                                    );
    bsp_wait( 500, BSP_TIME_MSEC );


    ret |= tlc59282_send_uint32_nl  (   SEG_C
                                    | ( SEG_C << 8)
                                    | ( SEG_C << 16 )
                                    | ( SEG_C << 24 )
                                    );
    ret |= tlc59282_send_uint16     (   SEG_C
                                    | ( SEG_C << 8)
                                    );
    bsp_wait( 500, BSP_TIME_MSEC );

    ret |= tlc59282_send_uint32_nl  (   SEG_D
                                    | ( SEG_D << 8)
                                    | ( SEG_D << 16 )
                                    | ( SEG_D << 24 )
                                    );
    ret |= tlc59282_send_uint16     (   SEG_D
                                    | ( SEG_D << 8)
                                    );
    bsp_wait( 500, BSP_TIME_MSEC );

    ret |= tlc59282_send_uint32_nl  (   SEG_E
                                    | ( SEG_E << 8)
                                    | ( SEG_E << 16 )
                                    | ( SEG_E << 24 )
                                    );
    ret |= tlc59282_send_uint16     (   SEG_E
                                    | ( SEG_E << 8)
                                    );
    bsp_wait( 500, BSP_TIME_MSEC );

    ret |= tlc59282_send_uint32_nl  (   SEG_F
                                    | ( SEG_F << 8)
                                    | ( SEG_F << 16 )
                                    | ( SEG_F << 24 )
                                    );
    ret |= tlc59282_send_uint16     (   SEG_F
                                    | ( SEG_F << 8)
                                    );
    bsp_wait( 500, BSP_TIME_MSEC );

    ret |= tlc59282_send_uint32_nl  (   SEG_G
                                    | ( SEG_G << 8)
                                    | ( SEG_G << 16 )
                                    | ( SEG_G << 24 )
                                    );
    ret |= tlc59282_send_uint16     (   SEG_G
                                    | ( SEG_G << 8)
                                    );
    bsp_wait( 500, BSP_TIME_MSEC );

    ret |= tlc59282_send_uint32_nl  (   SEG_H
                                    | ( SEG_H << 8)
                                    | ( SEG_H << 16 )
                                    | ( SEG_H << 24 )
                                    );
    ret |= tlc59282_send_uint16     (   SEG_H
                                    | ( SEG_H << 8)
                                    );
    bsp_wait( 500, BSP_TIME_MSEC );

    ret |= tlc59282_send_uint32_nl  ( 0xFFFFFFFF );
    ret |= tlc59282_send_uint16     ( 0xFFFF );
    bsp_wait( 1000, BSP_TIME_MSEC );

    ret |= tlc59282_clear();

    return ret;
}

status_t tlc59282_clear( void )
{
    status_t ret = STATUS_OK;
    uint8_t  i;

    for ( i = 0; i < TLC_NO_OF_SEG; i++ )
    {
        ret |= tlc59282_send_byte( tlc59282_ascii_tbl[0] );
    }

    if ( STATUS_OK == ret )
    {
        tlc59282_latch();
    }

    return ret;
}

void HAL_SPI_MspInit( SPI_HandleTypeDef* spi_base )
{
    GPIO_InitTypeDef gpio = {0};

    if ( SPI2 == spi_base->Instance )
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_SPI2_CLK_ENABLE();

        gpio.Pin    = GPIO_PIN_13 | GPIO_PIN_15;
        gpio.Mode   = GPIO_MODE_AF_PP;
        gpio.Pull   = GPIO_PULLDOWN;
        gpio.Speed  = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init( GPIOB, &gpio );

        gpio.Pin    = GPIO_PIN_14;
        gpio.Mode   = GPIO_MODE_INPUT;
        HAL_GPIO_Init( GPIOB, &gpio );
    }
}
