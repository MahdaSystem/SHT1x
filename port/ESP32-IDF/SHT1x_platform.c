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
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

static void
SHT1x_SetGPIO_OUT(gpio_num_t GPIO_Pad)
{
  gpio_reset_pin(GPIO_Pad);
  gpio_set_direction(GPIO_Pad, GPIO_MODE_OUTPUT);
}

static void
SHT1x_SetGPIO_IN_PU(gpio_num_t GPIO_Pad)
{
  gpio_reset_pin(GPIO_Pad);
  gpio_set_direction(GPIO_Pad, GPIO_MODE_INPUT);
  gpio_set_pull_mode(GPIO_Pad, GPIO_PULLUP_ONLY);
}


static void
SHT1x_Platform_DataDeInit(void)
{
  gpio_reset_pin(SHT1x_DATA_GPIO);
}

static void
SHT1x_Platform_DataConfigOut(void)
{
  SHT1x_SetGPIO_OUT(SHT1x_DATA_GPIO);
}

static void
SHT1x_Platform_DataConfigIn(void)
{
  SHT1x_SetGPIO_IN_PU(SHT1x_DATA_GPIO);
}

static void
SHT1x_Platform_DataWriteHigh(void)
{
  gpio_set_level(SHT1x_DATA_GPIO, 1);
}

static void
SHT1x_Platform_DataWriteLow(void)
{
  gpio_set_level(SHT1x_DATA_GPIO, 0);
}

static uint8_t
SHT1x_Platform_DataRead(void)
{
  return gpio_get_level(SHT1x_DATA_GPIO);
}

static void
SHT1x_Platform_SckDeInit(void)
{
  gpio_reset_pin(SHT1x_SCK_GPIO);
}

static void
SHT1x_Platform_SckConfigOut(void)
{
  SHT1x_SetGPIO_OUT(SHT1x_SCK_GPIO);
}

static void
SHT1x_Platform_SckWriteHigh(void)
{
  gpio_set_level(SHT1x_SCK_GPIO, 1);
}

static void
SHT1x_Platform_SckWriteLow(void)
{
  gpio_set_level(SHT1x_SCK_GPIO, 0);
}

static void
SHT1x_Platform_DelayMs(uint8_t Delay)
{
  vTaskDelay(Delay / portTICK_PERIOD_MS);
}

static void
SHT1x_Platform_DelayUs(uint8_t Delay)
{
  ets_delay_us(Delay);
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
