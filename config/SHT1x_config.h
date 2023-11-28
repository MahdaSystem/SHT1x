/**
 **********************************************************************************
 * @file   SHT1x_config.h
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  Project specific configurations
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
#ifndef _SHT1X_CONFIG_H_
#define _SHT1X_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif


/* Configurations ---------------------------------------------------------------*/
/**
 * @brief  Fahrenheit measurement option
 *         - 0: Disable Fahrenheit measurement function
 *         - 1: Enable Fahrenheit measurement function
 */
#define SHT1X_CONFIG_FAHRENHEIT_MEASUREMENT     0

/**
 * @brief  High resolution measurement option
 * @note   By setting this constant to 0, high resolution is selected
 *         - 0: Disable High resolution measurement
 *         - 1: Enable High resolution measurement
 */
#define SHT1X_CONFIG_RESOLUTION_CONTROL         0

/**
 * @brief  Determine how to measure the supply voltage
 *         - 0: The power supply voltage determine by SHT1x_SetPowVoltage function
 *         - 1: The power supply voltage is constant at 3.3 volts
 *         - 2: The power supply voltage is constant at 5 volts
 */
#define SHT1X_CONFIG_POWER_VOLTAGE_CONTROL      0

/**
 * @brief  Internal heater control option
 *         - 0: Disable internal heater control
 *         - 1: Enable internal heater control
 */
#define SHT1X_CONFIG_INTERNAL_HEATER_CONTROL    0



#ifdef __cplusplus
}
#endif

#endif //! _SHT1X_CONFIG_H_
