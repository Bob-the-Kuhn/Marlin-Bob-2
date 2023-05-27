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
