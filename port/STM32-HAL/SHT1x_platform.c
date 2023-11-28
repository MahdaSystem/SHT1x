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
#include "main.h"



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

static void
SHT1x_SetGPIO_OUT(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

static void
SHT1x_SetGPIO_IN_PU(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}


static void
SHT1x_Platform_PlatformInit(void)
{
  SHT1x_SetGPIO_OUT(SHT1x_SCK_GPIO, SHT1x_SCK_PIN);
  SHT1x_SetGPIO_OUT(SHT1x_DATA_GPIO, SHT1x_DATA_PIN);
}

static void
SHT1x_Platform_PlatformDeInit(void)
{
}

static void
SHT1x_Platform_DataConfigDir(uint8_t Dir)
{
  if (Dir)
    SHT1x_SetGPIO_OUT(SHT1x_DATA_GPIO, SHT1x_DATA_PIN);
  else
    SHT1x_SetGPIO_IN_PU(SHT1x_DATA_GPIO, SHT1x_DATA_PIN);
}

static void
SHT1x_Platform_DataWrite(uint8_t Level)
{
  HAL_GPIO_WritePin(SHT1x_DATA_GPIO, SHT1x_DATA_PIN, Level ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static uint8_t
SHT1x_Platform_DataRead(void)
{
  return HAL_GPIO_ReadPin(SHT1x_DATA_GPIO, SHT1x_DATA_PIN);
}

static void
SHT1x_Platform_SckWrite(uint8_t Level)
{
  HAL_GPIO_WritePin(SHT1x_SCK_GPIO, SHT1x_SCK_PIN, Level ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void
SHT1x_Platform_DelayMs(uint8_t Delay)
{
  HAL_Delay(Delay);
}

static void
SHT1x_Platform_DelayUs(uint8_t Delay)
{
  uint32_t clk_freq = HAL_RCC_GetSysClockFreq();
  uint32_t ticks = (Delay * clk_freq) / 1000000;
  for (uint32_t i = 0; i < ticks; i++)
  {
    __NOP();
  }
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
