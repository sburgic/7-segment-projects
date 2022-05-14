/**
 ** Name
 **   interrupt.h
 **
 ** Purpose
 **   IRQ routines
 **
 ** Revision
 **   19-Apr-2020 (SSB) [] Initial
 **/

#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <stm32f1xx_hal.h>

void NMI_Handler( void );
void HardFault_Handler( void );
void MemManage_Handler( void );
void BusFault_Handler( void );
void UsageFault_Handler( void );
void DebugMon_Handler( void );
void SysTick_Handler( void );
void TIM1_UP_TIM16_IRQHandler( void );
void TIM2_IRQHandler( void );

#endif /*__INTERRUPT_H__ */
