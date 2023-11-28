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
#ifndef _SHT1X_PLATFORM_H
#define _SHT1X_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ---------------------------------------------------------------------*/
#include <stdint.h>
#include "SHT1x.h"


/* Functionality Options --------------------------------------------------------*/
/**
 * @brief  Specify IO Pins of STM32 connected to TM1638
 */
#define SHT1x_SCK_GPIO    GPIOA
#define SHT1x_SCK_PIN     GPIO_PIN_0
#define SHT1x_DATA_GPIO   GPIOA
#define SHT1x_DATA_PIN    GPIO_PIN_1



/**
 ==================================================================================
                               ##### Functions #####                               
 ==================================================================================
 */

/**
 * @brief  Initialize platform device to communicate SHT1x.
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 */
SHT1x_Result_t
SHT1x_Platform_Init(SHT1x_Handler_t *Handler);



#ifdef __cplusplus
}
#endif


#endif
