/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */


/**
 *
 * This file's purpose is to provide the STM32F7xx_boot_ldscript.ld access
 * to the VECTOR_TABLE_OFFSET flag/value defined in the environment.  This
 * allows the bootloader offset address to be set by the environment.
 *
 */


#include <stdint.h>
#include <inttypes.h>
#include "stm32f7xx_hal_rcc.h"
#include "stm32f7xx_hal_pwr.h"
#include "stm32f7xx_hal_flash_ex.h"

//C:\Users\bobku\Documents\GitHub\Marlin-Bob-2\Marlin\src\HAL\STM32\MinSerial.cpp
//C:\Users\bobku\Documents\GitHub\Marlin-Bob-2\Marlin\src\HAL\shared\cpu_exception\exception_arm.cpp
//C:\Users\bobku\Documents\GitHub\Marlin-Bob-2\Marlin\src\HAL\STM32\STM32F7\STM32F7xx_boot_stub.c
//#include "../../shared/cpu_exception/exception_hook.h"
//#include "../../shared/backtrace/backtrace.h"
//#include "../../shared/MinSerial.h"

//// Common exception frame for ARM, should work for all ARM CPU
//// Described here (modified for convenience): https://interrupt.memfault.com/blog/cortex-m-fault-debug
//struct __attribute__((packed)) ContextStateFrame {
//  uint32_t r0;
//  uint32_t r1;
//  uint32_t r2;
//  uint32_t r3;
//  uint32_t r12;
//  uint32_t lr;
//  uint32_t pc;
//  uint32_t xpsr;
//};
//
//
//
//void CommonHandler_C(ContextStateFrame * frame, unsigned long lr, unsigned long cause);
void CommonHandler_ASM();

  volatile unsigned long stacked_r0 ;
  volatile unsigned long stacked_r1 ;
  volatile unsigned long stacked_r2 ;
  volatile unsigned long stacked_r3 ;
  volatile unsigned long stacked_r12 ;
  volatile unsigned long stacked_lr ;
  volatile unsigned long stacked_pc ;
  volatile unsigned long stacked_psr ;
  volatile unsigned long _CFSR ;
  volatile unsigned long _HFSR ;
  volatile unsigned long _DFSR ;
  volatile unsigned long _AFSR ;
  volatile unsigned long _BFAR ;
  volatile unsigned long _MMAR ;

void hard_fault_handler_c(unsigned long *hardfault_args){

  // see https://blog.nathantsoi.com/article/stm32-hardfault-debugging/
  //     https://interrupt.memfault.com/blog/cortex-m-hardfault-debug

  //volatile unsigned long __attribute__((section("no_init"))) stacked_r0 ;
  //volatile unsigned long __attribute__((section("no_init"))) stacked_r1 ;
  //volatile unsigned long __attribute__((section("no_init"))) stacked_r2 ;
  //volatile unsigned long __attribute__((section("no_init"))) stacked_r3 ;
  //volatile unsigned long __attribute__((section("no_init"))) stacked_r12 ;
  //volatile unsigned long __attribute__((section("no_init"))) stacked_lr ;
  //volatile unsigned long __attribute__((section("no_init"))) stacked_pc ;
  //volatile unsigned long __attribute__((section("no_init"))) stacked_psr ;
  //volatile unsigned long __attribute__((section("no_init"))) _CFSR ;
  //volatile unsigned long __attribute__((section("no_init"))) _HFSR ;
  //volatile unsigned long __attribute__((section("no_init"))) _DFSR ;
  //volatile unsigned long __attribute__((section("no_init"))) _AFSR ;
  //volatile unsigned long __attribute__((section("no_init"))) _BFAR ;
  //volatile unsigned long __attribute__((section("no_init"))) _MMAR ;

  stacked_r0 = ((unsigned long)hardfault_args[0]) ;
  stacked_r1 = ((unsigned long)hardfault_args[1]) ;
  stacked_r2 = ((unsigned long)hardfault_args[2]) ;
  stacked_r3 = ((unsigned long)hardfault_args[3]) ;
  stacked_r12 = ((unsigned long)hardfault_args[4]) ;
  stacked_lr = ((unsigned long)hardfault_args[5]) ;
  stacked_pc = ((unsigned long)hardfault_args[6]) ;
  stacked_psr = ((unsigned long)hardfault_args[7]) ;

  // Configurable Fault Status Register
  // Consists of MMSR, BFSR and UFSR
  _CFSR = (*((volatile unsigned long *)(0xE000ED28))) ;

  // Hard Fault Status Register
  _HFSR = (*((volatile unsigned long *)(0xE000ED2C))) ;

  // Debug Fault Status Register
  _DFSR = (*((volatile unsigned long *)(0xE000ED30))) ;

  // Auxiliary Fault Status Register
  _AFSR = (*((volatile unsigned long *)(0xE000ED3C))) ;

  // Read the Fault Address Registers. These may not contain valid values.
  // Check BFARVALID/MMARVALID to see if they are valid values
  // MemManage Fault Address Register
  _MMAR = (*((volatile unsigned long *)(0xE000ED34))) ;
  // Bus Fault Address Register
  _BFAR = (*((volatile unsigned long *)(0xE000ED38))) ;

 // __asm("BKPT #0\n") ; // Break into the debugger
 CommonHandler_ASM();
}




void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  //if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  //{
  //  Error_Handler();
  //}

  /** Activate the Over-Drive mode
  */
  HAL_PWREx_EnableOverDrive();
  //if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  //{
  //  Error_Handler();
  //}

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
  //if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  //{
  //  Error_Handler();
  //}
}



#if (defined(STM32F7xx) && defined(VECTOR_TABLE_OFFSET))

/**
  * @brief Provides a tick value in millisecond.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @retval tick value
  */
//uint32_t HAL_GetTick(void)
//{
//  #define SysTick_CTRL_COUNTFLAG_Pos         16U                                            /*!< SysTick CTRL: COUNTFLAG Position */
//  #define SysTick_CTRL_COUNTFLAG_Msk         (1UL << SysTick_CTRL_COUNTFLAG_Pos)            /*!< SysTick CTRL: COUNTFLAG Mask */
//
//  if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)  HAL_IncTick();
//  return uwTick;
//}
//

//#ifdef __cplusplus
//extern "C" {
//#endif

// provide method for loader file to access #define values

//int FLASH_OFFSET = (int)LD_FLASH_OFFSET;
//__asm__ __volatile__("" :: "m" (FLASH_OFFSET));
//

//typedef struct foo
//{
//int FLASH_OFFSET;
//} foo;
//
//foo var __attribute__((__used__)) = {(int)LD_FLASH_OFFSET};

//static const int flash_offset[] __attribute__((used)) = {(int)LD_FLASH_OFFSET, (int)(LD_FLASH_OFFSET >> 16)};
//static const int flash_offset[2] __attribute__((used)) = {(int)LD_FLASH_OFFSET, (int)(LD_FLASH_OFFSET >> 16)};


//static const int flash_offset[2] __attribute__((section(".unused"))) = {(int)LD_FLASH_OFFSET, (int)(LD_FLASH_OFFSET >> 16)};


//#pragma GCC push_options
//#pragma GCC optimize ("O0")
//static const int flash_offset[2] = {(int)LD_FLASH_OFFSET, (int)(LD_FLASH_OFFSET >> 16)};
//static const int flash_offset_2 = 0x8000;
//#pragma GCC pop_options

//int *temp_flash;
//&temp_flash = 0x20000000;
//*temp_flash = LD_FLASH_OFFSET;

//volatile uint32_t* temp_flash = (uint32_t*)0x20000000UL;
//*temp_flash = (uint32_t)(LD_FLASH_OFFSET);





//int *temp;
//temp = 0x20000000;
//*temp = FLASH_OFFSET;

//extern int _LD_FLASH_OFFSET;
//int *p;
//p = (int*)&_LD_FLASH_OFFSET;
//*p = VECTOR_TABLE_OFFSET;

//#ifdef __cplusplus
//}
//#endif

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

void init(void)
{
  hw_config_init();

  __ASM volatile ("cpsie i" : : : "memory");  // enable IRQs
}

#ifdef __cplusplus
}
#endif

#endif // VECTOR_TABLE_OFFSET
