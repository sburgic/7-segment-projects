/**
 ** Name
 **   interrupt.c
 **
 ** Purpose
 **   IRQ routines
 **
 ** Revision
 **   19-Apr-2020 (SSB) [] Initial
 **/

#include "interrupt.h"

#include "led.h"
#include "time.h"

void NMI_Handler( void )
{
}

void HardFault_Handler( void )
{
    while (1)
    {;}
}

void MemManage_Handler( void )
{
    while (1)
    {;}
}

void BusFault_Handler( void )
{
    while (1)
    {;}
}

void UsageFault_Handler( void )
{
    while (1)
    {;}
}

void DebugMon_Handler( void )
{
    while (1)
    {;}
}

void TIM1_UP_TIM16_IRQHandler( void )
{
    bsp_tim1_irq_hdl();
    led_blink_alive();
}

void TIM2_IRQHandler( void )
{
    bsp_tmr_slave_irq_hdl();
}
