/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

#include "env_validate.h"


   //               virtual com port
  // USART 3        USART 1       SPI 2
  // PB9  AF7 TX    PA9  AF7 TX   PB12 AF5 SS
  // PB8  AF7 Rx    PA10 AF7 Rx   PB13 AF5 CLK
  //                              PB14 AF5 MISO
  //                              PB15 AF5 MOSI
  //
  // PB5 SD_detect
  //
  // PA5 LED, green (built in)
  // PA7 LED, yellow
  //
  // PC13 button
  //
  // PA13 SWDIO  AF0
  // PA14 SWCLK  AF0
  // PB3  SWO    AF0


  #define SDSS                       PB12
  #define SD_SS_PIN                  SDSS
  #define SD_SCK_PIN                 PB13
  #define SD_MISO_PIN                PB14
  #define SD_MOSI_PIN                PB15
  #define SD_DETECT_PIN              PB15

  #define SD_DETECT_STATE HIGH

  #define LED_PIN                    PA5

  //#define SOFTWARE_SPI


//  ALL PIN ASSIGNMENTS FROM HERE ON ARE RANDOM.  THEY ARE JUST
//  HERE TO ALLOW THE BOARD TO COMPILE.

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD    2000

//
// EEPROM
//
#if ANY(NO_EEPROM_SELECTED, FLASH_EEPROM_EMULATION)
  #undef NO_EEPROM_SELECTED
  #ifndef FLASH_EEPROM_EMULATION
    #define FLASH_EEPROM_EMULATION
  #endif
  #define EEPROM_PAGE_SIZE      (0x800UL) // 2K
  #define EEPROM_START_ADDRESS  (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
  #define MARLIN_EEPROM_SIZE    EEPROM_PAGE_SIZE
#endif


//
// Servos
//
#define SERVO0_PIN                          PB1   // PROBE

//
// Probe enable
//
#if ENABLED(PROBE_ENABLE_DISABLE)
  #ifndef PROBE_ENABLE_PIN
    #define PROBE_ENABLE_PIN          SERVO0_PIN
  #endif
#endif

//
// Trinamic Stallguard pins
//
#define X_DIAG_PIN                          PA0   // MIN1
#define Y_DIAG_PIN                          PA0   // MIN2
#define Z_DIAG_PIN                          PA0   // MIN3
#define Z2_DIAG_PIN                         PC0   // MIN4
#define E0_DIAG_PIN                         PC1   // MIN5
#define E1_DIAG_PIN                         PC2   // MIN6

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PB2   // PROBE
  //#define Z_MIN_PROBE_PIN                 PA0   // IND-PROBE (with adjustable voltage & pullup set via jumpers)
#endif

//
// Limit Switches
//
#ifdef X_STALL_SENSITIVITY
  #define X_STOP_PIN                  X_DIAG_PIN
  #if X_HOME_TO_MIN
    #define X_MAX_PIN                E0_DIAG_PIN  // MIN5
  #else
    #define X_MIN_PIN                E0_DIAG_PIN  // MIN5
  #endif
#elif NEEDS_X_MINMAX
  #ifndef X_MIN_PIN
    #define X_MIN_PIN                 X_DIAG_PIN  // MIN1
  #endif
  #ifndef X_MAX_PIN
    #define X_MAX_PIN                E0_DIAG_PIN  // MIN5
  #endif
#else
  #define X_STOP_PIN                  X_DIAG_PIN  // MIN1
#endif

#ifdef Y_STALL_SENSITIVITY
  #define Y_STOP_PIN                  Y_DIAG_PIN
  #if Y_HOME_TO_MIN
    #define Y_MAX_PIN                E1_DIAG_PIN  // MIN6
  #else
    #define Y_MIN_PIN                E1_DIAG_PIN  // MIN6
  #endif
#elif NEEDS_Y_MINMAX
  #ifndef Y_MIN_PIN
    #define Y_MIN_PIN                 Y_DIAG_PIN  // MIN2
  #endif
  #ifndef Y_MAX_PIN
    #define Y_MAX_PIN                E1_DIAG_PIN  // MIN6
  #endif
#else
  #define Y_STOP_PIN                  Y_DIAG_PIN  // MIN2
#endif

#define Z_STOP_PIN                    Z_DIAG_PIN  // MIN3

//
// Filament Runout Sensors
//
#define FIL_RUNOUT_PIN                      PC1   // MIN5
#define FIL_RUNOUT2_PIN                     PC2   // MIN6

#ifndef PS_ON_PIN
  #define PS_ON_PIN                         PC3   // PS-ON
#endif

//
// Steppers
//
#define X_STEP_PIN                          PA2   // M1
#define X_DIR_PIN                           PB4
#define X_ENABLE_PIN                        PC11
#ifndef X_CS_PIN
  #define X_CS_PIN                          PC10
#endif

#define Y_STEP_PIN                          PA0  // M2
#define Y_DIR_PIN                           PA0
#define Y_ENABLE_PIN                        PB3
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PA0
#endif

#define Z_STEP_PIN                          PA0   // M3
#define Z_DIR_PIN                           PA15
#define Z_ENABLE_PIN                        PA0
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PA0
#endif

#define Z2_STEP_PIN                         PF0   // M4
#define Z2_DIR_PIN                          PD2
#define Z2_ENABLE_PIN                       PF1
#ifndef Z2_CS_PIN
  #define Z2_CS_PIN                         PA15
#endif

#define E0_STEP_PIN                         PC9   // M5
#define E0_DIR_PIN                          PC8
#define E0_ENABLE_PIN                       PA15
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PA4
#endif



//
// Temperature Sensors
//
#define TEMP_BED_PIN                        PA0   // TB
#define TEMP_0_PIN                          PA1   // TH0
#define TEMP_1_PIN                          PA2   // TH1
#define TEMP_2_PIN                          PA3   // TH2
#define TEMP_3_PIN                          PA4   // TH3

//
// Heaters / Fans
//
#define HEATER_BED_PIN                      PB7   // BED-OUT
#define HEATER_0_PIN                        PA3   // HE0
#define HEATER_1_PIN                        PB5   // HE1
#define HEATER_2_PIN                        PB6   // HE2
#define HEATER_3_PIN                        PA1   // HE3

#define FAN0_PIN                            PA6   // FAN0
#define FAN1_PIN                            PA0   // FAN1
#define FAN2_PIN                            PC12  // FAN2
#define FAN3_PIN                            PA5   // FAN3
#define FAN4_PIN                            PA4   // FAN4
#define FAN5_PIN                            PB8   // FAN5
#define FAN6_PIN                            PB9   // FAN6

//
// SD Support
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION            ONBOARD
#endif


