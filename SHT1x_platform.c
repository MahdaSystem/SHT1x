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

#if defined(SHT1x_PLATFORM_AVR)
#include <avr/io.h>
#define F_CPU SHT1x_AVR_CLK
#include <util/delay.h>
#elif defined(SHT1x_PLATFORM_STM32)
#include "main.h"
#elif defined(SHT1x_PLATFORM_ESP32_IDF)
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"
#endif



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */
#if defined(SHT1x_PLATFORM_STM32)
void SHT1x_SetGPIO_OUT(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void SHT1x_SetGPIO_IN_PU(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
#elif defined(SHT1x_PLATFORM_ESP32_IDF)
void SHT1x_SetGPIO_OUT(uint32_t GPIO_Pad)
{
  gpio_config_t SHT1x_GPIO_CONF;
  SHT1x_GPIO_CONF.pin_bit_mask = GPIO_Pad;
  SHT1x_GPIO_CONF.mode = GPIO_MODE_OUTPUT;
  SHT1x_GPIO_CONF.pull_up_en = GPIO_PULLUP_DISABLE;
  SHT1x_GPIO_CONF.pull_down_en = GPIO_PULLDOWN_DISABLE;
  SHT1x_GPIO_CONF.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&SHT1x_GPIO_CONF);
}

void SHT1x_SetGPIO_IN_PU(uint32_t GPIO_Pad)
{
  gpio_config_t SHT1x_GPIO_CONF;
  SHT1x_GPIO_CONF.pin_bit_mask = GPIO_Pad;
  SHT1x_GPIO_CONF.mode = GPIO_MODE_DEF_INPUT;
  SHT1x_GPIO_CONF.pull_up_en = GPIO_PULLUP_ENABLE;
  SHT1x_GPIO_CONF.pull_down_en = GPIO_PULLDOWN_DISABLE;
  SHT1x_GPIO_CONF.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&SHT1x_GPIO_CONF);
}
#endif

void SHT1x_Platform_DataDeInit(void)
{
#if defined(SHT1x_PLATFORM_AVR)
  SHT1x_DATA_PORT &= ~(1<<SHT1x_DATA_NUM);
  SHT1x_DATA_DDR &= ~(1<<SHT1x_DATA_NUM);
#elif defined(SHT1x_PLATFORM_STM32)
#elif defined(SHT1x_PLATFORM_ESP32_IDF)
  gpio_reset_pin(SHT1x_DATA_GPIO);
#endif
}

void SHT1x_Platform_DataConfigOut(void)
{
#if defined(SHT1x_PLATFORM_AVR)
  SHT1x_DATA_DDR |= (1<<SHT1x_DATA_NUM);
#elif defined(SHT1x_PLATFORM_STM32)
  SHT1x_SetGPIO_OUT(SHT1x_DATA_GPIO, SHT1x_DATA_PIN);
#elif defined(SHT1x_PLATFORM_ESP32_IDF)
  SHT1x_SetGPIO_OUT(1<<SHT1x_DATA_GPIO);
#endif
}

void SHT1x_Platform_DataConfigIn(void)
{
#if defined(SHT1x_PLATFORM_AVR)
  SHT1x_DATA_DDR &= ~(1<<SHT1x_DATA_NUM);
#elif defined(SHT1x_PLATFORM_STM32)
  SHT1x_SetGPIO_IN_PU(SHT1x_DATA_GPIO, SHT1x_DATA_PIN);
#elif defined(SHT1x_PLATFORM_ESP32_IDF)
  SHT1x_SetGPIO_IN_PU(1<<SHT1x_DATA_GPIO);
#endif
}

void SHT1x_Platform_DataWriteHigh(void)
{
#if defined(SHT1x_PLATFORM_AVR)
  SHT1x_DATA_PORT |= (1<<SHT1x_DATA_NUM);
#elif defined(SHT1x_PLATFORM_STM32)
  HAL_GPIO_WritePin(SHT1x_DATA_GPIO, SHT1x_DATA_PIN, GPIO_PIN_SET);
#elif defined(SHT1x_PLATFORM_ESP32_IDF)
  gpio_set_level(SHT1x_DATA_GPIO, 1);
#endif
}

void SHT1x_Platform_DataWriteLow(void)
{
#if defined(SHT1x_PLATFORM_AVR)
  SHT1x_DATA_PORT &= ~(1<<SHT1x_DATA_NUM);
#elif defined(SHT1x_PLATFORM_STM32)
  HAL_GPIO_WritePin(SHT1x_DATA_GPIO, SHT1x_DATA_PIN, GPIO_PIN_RESET);
#elif defined(SHT1x_PLATFORM_ESP32_IDF)
  gpio_set_level(SHT1x_DATA_GPIO, 0);
#endif
}

uint8_t SHT1x_Platform_DataRead(void)
{
  uint8_t Result = 1;
#if defined(SHT1x_PLATFORM_AVR)
  Result = (DATA_PIN >> DATA_NUM) & 0x01;
#elif defined(SHT1x_PLATFORM_STM32)
  Result = HAL_GPIO_ReadPin(SHT1x_DATA_GPIO, SHT1x_DATA_PIN);
#elif defined(SHT1x_PLATFORM_ESP32_IDF)
  Result = gpio_get_level(SHT1x_DATA_GPIO);
#endif
  return Result;
}



void SHT1x_Platform_SckDeInit(void)
{
#if defined(SHT1x_PLATFORM_AVR)
  SHT1x_SCK_PORT &= ~(1<<SHT1x_SCK_NUM);
  SHT1x_SCK_DDR &= ~(1<<SHT1x_SCK_NUM);
#elif defined(SHT1x_PLATFORM_STM32)
#elif defined(SHT1x_PLATFORM_ESP32_IDF)
  gpio_reset_pin(SHT1x_SCK_GPIO);
#endif
}

void SHT1x_Platform_SckConfigOut(void)
{
#if defined(SHT1x_PLATFORM_AVR)
  SHT1x_SCK_DDR |= (1<<SHT1x_SCK_NUM);
#elif defined(SHT1x_PLATFORM_STM32)
  SHT1x_SetGPIO_OUT(SHT1x_SCK_GPIO, SHT1x_SCK_PIN);
#elif defined(SHT1x_PLATFORM_ESP32_IDF)
  SHT1x_SetGPIO_OUT(1<<SHT1x_SCK_GPIO);
#endif
}

void SHT1x_Platform_SckWriteHigh(void)
{
#if defined(SHT1x_PLATFORM_AVR)
  SHT1x_SCK_PORT |= (1<<SHT1x_SCK_NUM);
#elif defined(SHT1x_PLATFORM_STM32)
  HAL_GPIO_WritePin(SHT1x_SCK_GPIO, SHT1x_SCK_PIN, GPIO_PIN_SET)
#elif defined(SHT1x_PLATFORM_ESP32_IDF)
  gpio_set_level(SHT1x_SCK_GPIO, 1);
#endif
}

void SHT1x_Platform_SckWriteLow(void)
{
#if defined(SHT1x_PLATFORM_AVR)
  SHT1x_SCK_PORT &= ~(1<<SHT1x_SCK_NUM);
#elif defined(SHT1x_PLATFORM_STM32)
  HAL_GPIO_WritePin(SHT1x_SCK_GPIO, SHT1x_SCK_PIN, GPIO_PIN_RESET);
#elif defined(SHT1x_PLATFORM_ESP32_IDF)
  gpio_set_level(SHT1x_SCK_GPIO, 0);
#endif
}


void SHT1x_Platform_DelayMs(uint8_t Delay)
{
#if defined(SHT1x_PLATFORM_AVR)
  for (; Delay; --Delay)
    _delay_ms(1);
#elif defined(SHT1x_PLATFORM_STM32)
  HAL_Delay(Delay);
#elif defined(SHT1x_PLATFORM_ESP32_IDF)
  vTaskDelay(Delay / portTICK_PERIOD_MS);
#endif
}

void SHT1x_Platform_DelayUs(uint8_t Delay)
{
#if defined(SHT1x_PLATFORM_AVR)
  for (; Delay; --Delay)
    _delay_us(1);
#elif defined(SHT1x_PLATFORM_STM32)
  for(uint32_t DelayCounter = 0; DelayCounter<100*Delay; DelayCounter++) DelayCounter = DelayCounter;
#elif defined(SHT1x_PLATFORM_ESP32_IDF)
  ets_delay_us(Delay);
#endif
}



/**
 ==================================================================================
                            ##### Public Functions #####                           
 ==================================================================================
 */

/**
 * @brief  Initialize platfor device to communicate SHT1x.
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 */
SHT1x_Result_t
SHT1x_Platform_Init(SHT1x_Handler_t *Handler)
{
  Handler->DataDeInit = SHT1x_Platform_DataDeInit;
  Handler->DataConfigOut = SHT1x_Platform_DataConfigOut;
  Handler->DataConfigIn = SHT1x_Platform_DataConfigIn;
  Handler->DataWriteHigh = SHT1x_Platform_DataWriteHigh;
  Handler->DataWriteLow = SHT1x_Platform_DataWriteLow;
  Handler->DataRead = SHT1x_Platform_DataRead;
  Handler->SckDeInit = SHT1x_Platform_SckDeInit;
  Handler->SckConfigOut = SHT1x_Platform_SckConfigOut;
  Handler->SckWriteHigh = SHT1x_Platform_SckWriteHigh;
  Handler->SckWriteLow = SHT1x_Platform_SckWriteLow;
  Handler->DelayMs = SHT1x_Platform_DelayMs;
  Handler->DelayUs = SHT1x_Platform_DelayUs;

  return SHT1x_OK;
}
