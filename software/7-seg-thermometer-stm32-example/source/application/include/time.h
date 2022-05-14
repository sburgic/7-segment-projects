/**
 ** Name
 **   time.h
 **
 ** Purpose
 **   Time routines
 **
 ** Revision
 **   20-Apr-2020 (SSB) [] Initial
 **/

#ifndef __TIME_H__
#define __TIME_H__

#include "ptypes.h"

#include <stm32f1xx_hal.h>

#define TMR_OS                  TIM1
#define TMR_OS_CLK_ENABLE()     __HAL_RCC_TIM1_CLK_ENABLE()
#define TMR_OS_IQRn             TIM1_UP_TIM16_IRQn

#define TMR_MASTER              TIM3
#define TMR_MASTER_CLK_ENABLE() __HAL_RCC_TIM3_CLK_ENABLE()
#define TMR_MASTER_IQRn         TIM3_IRQn

#define TMR_SLAVE               TIM2
#define TMR_SLAVE_CLK_ENABLE()  __HAL_RCC_TIM2_CLK_ENABLE()
#define TMR_SLAVE_IRQn          TIM2_IRQn

typedef enum
{
    BSP_TIME_USEC = 1,
    BSP_TIME_MSEC = 1000,
    BSP_TIME_SEC  = 1000000
} Bsp_Time_Base;

typedef uint64_t Bsp_Time;

HAL_StatusTypeDef bsp_tmr_init( void );
void bsp_get_time( Bsp_Time* tv );
void bsp_tmr_slave_irq_hdl( void );
void bsp_hi_sys_time_inc( void );
void bsp_wait( Bsp_Time time, Bsp_Time_Base base );
bool_t bsp_is_timeout( Bsp_Time timeout );
void bsp_set_timeout( Bsp_Time      time
                    , Bsp_Time_Base base
                    , Bsp_Time*     timeout
                    );
void bsp_tim1_irq_hdl( void );

#endif /* __TIME_H__ */
