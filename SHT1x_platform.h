/**
 **********************************************************************************
 * @file   SHT1x_platform.h
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  Platform dependent part of SHT1x Library
 **********************************************************************************
 *
 * Copyright (c) 2021 Mahda Embedded System (MIT License)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************************************
 */
  
/* Define to prevent recursive inclusion ----------------------------------------*/
#ifndef	_SHT1X_PLATFORM_H
#define _SHT1X_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ---------------------------------------------------------------------*/
#include <stdint.h>
#include "SHT1x.h"


/* Functionality Options --------------------------------------------------------*/
/**
 * @brief  Specify the target platform
 * @note   Uncomment the line below according to the target platform
 */ 
// #define SHT1x_PLATFORM_AVR
// #define SHT1x_PLATFORM_STM32       // HAL Functions
// #define SHT1x_PLATFORM_ESP32_IDF   // ESP-IDF


#if defined(SHT1x_PLATFORM_AVR)
/**
 * @brief  Specify IO Pins of AVR connected to TM1638
 */
#define SHT1x_AVR_CLK   8000000UL
#define SHT1x_DATA_DDR  DDRA
#define SHT1x_DATA_PORT PORTA
#define SHT1x_DATA_PIN  PINA
#define SHT1x_DATA_NUM  0
#define SHT1x_SCK_DDR   DDRA
#define SHT1x_SCK_PORT  PORTA
#define SHT1x_SCK_NUM   1
#endif

#if defined(SHT1x_PLATFORM_STM32)
/**
 * @brief  Specify IO Pins of STM32 connected to TM1638
 */
#define SHT1x_SCK_GPIO    GPIOA
#define SHT1x_SCK_PIN     GPIO_PIN_0
#define SHT1x_DATA_GPIO   GPIOA
#define SHT1x_DATA_PIN    GPIO_PIN_1
#endif

#if defined(SHT1x_PLATFORM_ESP32_IDF)
/**
 * @brief  Specify IO Pins of ESP32 connected to SHT1x
 */
#define SHT1x_SCK_GPIO		GPIO_NUM_17
#define SHT1x_DATA_GPIO		GPIO_NUM_18
#endif



/**
 ==================================================================================
                               ##### Functions #####                               
 ==================================================================================
 */

/**
 * @brief  Initialize platfor device to communicate SHT1x.
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 */
SHT1x_Result_t
SHT1x_Platform_Init(SHT1x_Handler_t *Handler);



#ifdef __cplusplus
}
#endif


#endif
