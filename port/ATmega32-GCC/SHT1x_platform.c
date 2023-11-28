/**
 **********************************************************************************
 * @file   SHT1x_platform.c
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
  
/* Includes ---------------------------------------------------------------------*/
#include "SHT1x_platform.h"
#include <avr/io.h>
#include <util/delay.h>



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

static void
SHT1x_Platform_PlatformInit(void)
{
  SHT1x_SCK_DDR |= (1<<SHT1x_SCK_NUM);
  SHT1x_DATA_DDR |= (1<<SHT1x_DATA_NUM);
}

static void
SHT1x_Platform_PlatformDeInit(void)
{
  SHT1x_SCK_PORT &= ~(1<<SHT1x_SCK_NUM);
  SHT1x_SCK_DDR &= ~(1<<SHT1x_SCK_NUM);

  SHT1x_DATA_PORT &= ~(1<<SHT1x_DATA_NUM);
  SHT1x_DATA_DDR &= ~(1<<SHT1x_DATA_NUM);
}

static void
SHT1x_Platform_DataConfigDir(uint8_t Dir)
{
  if (Dir)
    SHT1x_DATA_DDR |= (1<<SHT1x_DATA_NUM);
  else
    SHT1x_DATA_DDR &= ~(1<<SHT1x_DATA_NUM);
}

static void
SHT1x_Platform_DataWrite(uint8_t Level)
{
  if (Level)
    SHT1x_DATA_PORT |= (1<<SHT1x_DATA_NUM);
  else
    SHT1x_DATA_PORT &= ~(1<<SHT1x_DATA_NUM);
}

static uint8_t
SHT1x_Platform_DataRead(void)
{
  return (SHT1x_DATA_PIN >> SHT1x_DATA_NUM) & 0x01;
}

static void
SHT1x_Platform_SckWrite(uint8_t Level)
{
  if (Level)
    SHT1x_SCK_PORT |= (1<<SHT1x_SCK_NUM);
  else
    SHT1x_SCK_PORT &= ~(1<<SHT1x_SCK_NUM);
}

static void
SHT1x_Platform_DelayMs(uint8_t Delay)
{
  for (; Delay; --Delay)
    _delay_ms(1);
}

static void
SHT1x_Platform_DelayUs(uint8_t Delay)
{
  for (; Delay; --Delay)
    _delay_us(1);
}



/**
 ==================================================================================
                            ##### Public Functions #####                           
 ==================================================================================
 */

/**
 * @brief  Initialize platform device to communicate SHT1x.
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 */
SHT1x_Result_t
SHT1x_Platform_Init(SHT1x_Handler_t *Handler)
{
  Handler->PlatformInit = SHT1x_Platform_PlatformInit;
  Handler->PlatformDeInit = SHT1x_Platform_PlatformDeInit;
  Handler->DataConfigDir = SHT1x_Platform_DataConfigDir;
  Handler->DataWrite = SHT1x_Platform_DataWrite;
  Handler->DataRead = SHT1x_Platform_DataRead;
  Handler->SckWrite = SHT1x_Platform_SckWrite;
  Handler->DelayMs = SHT1x_Platform_DelayMs;
  Handler->DelayUs = SHT1x_Platform_DelayUs;

  return SHT1x_OK;
}
