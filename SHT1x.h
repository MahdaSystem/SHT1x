/**
 **********************************************************************************
 * @file   SHT1x.h
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  SHT1x series sensor driver
 *         Functionalities of the this file:
 *          + Read Temperature in Raw data, Celsius and Fahrenheit
 *          + Read Humidity in Raw data and percentage
 *          + Config sensor resolution
 *          + Control internal heater
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
#ifndef _SHT1X_H
#define _SHT1X_H

#ifdef __cplusplus
extern "C"
{
#endif


/* Includes ---------------------------------------------------------------------*/
#include <stdint.h>


/* Exported Data Types ----------------------------------------------------------*/
/**
 * @brief  Set and Get sensor resolution data type
 */
typedef enum SHT1x_Resolution_e
{
  SHT1x_LowResolution = 0,
  SHT1x_HighResolution = 1
} SHT1x_Resolution_t;

/**
 * @brief  Handler data type
 * @note   User must initialize this this functions before using library:
 *         - DataDeInit
 *         - DataConfigOut
 *         - DataConfigIn
 *         - DataWriteHigh
 *         - DataWriteLow
 *         - DataRead
 *         - SckDeInit
 *         - SckConfigOut
 *         - SckWriteHigh
 *         - SckWriteLow
 *         - DelayMs
 *         - DelayUs
 */
typedef struct SHT1x_Handler_s
{
  float D1Celsius;
  float D1Fahrenheit;
  SHT1x_Resolution_t ResolutionStatus;

  // Uninitialize the GPIO that connected to DATA PIN of SHT1x
  void (*DataDeInit)(void);
  // Config the GPIO that connected to DATA PIN of SHT1x as output
  void (*DataConfigOut)(void);
  // Config the GPIO that connected to DATA PIN of SHT1x as input
  void (*DataConfigIn)(void);
  // Set the GPIO that connected to DATA PIN of SHT1x
  void (*DataWriteHigh)(void);
  // Reset the GPIO that connected to DATA PIN of SHT1x
  void (*DataWriteLow)(void);
  // Read the GPIO that connected to DATA PIN of SHT1x
  uint8_t (*DataRead)(void);

  // Uninitialize the GPIO that connected to SCK PIN of SHT1x
  void (*SckDeInit)(void);
  // Config the GPIO that connected to SCK PIN of SHT1x as output
  void (*SckConfigOut)(void);
  // Set the GPIO that connected to DATA PIN of SHT1x
  void (*SckWriteHigh)(void);
  // Reset the GPIO that connected to DATA PIN of SHT1x
  void (*SckWriteLow)(void);

  // Delay (ms)
  void (*DelayMs)(uint8_t);
  // Delay (us)
  void (*DelayUs)(uint8_t);
} SHT1x_Handler_t;

/**
 * @brief  Sample data type
 */
typedef struct SHT1x_Sample_s
{
  uint16_t TempRaw;
  uint16_t HumRaw;
  float TempCelsius;
  float TempFahrenheit;
  float HumidityPercent;
} SHT1x_Sample_t;

/**
 * @brief  Library functions result data type
 */
typedef enum SHT1x_Result_e
{
  SHT1x_OK = 0,
  SHT1x_FAIL = 1,
  SHT1x_TIME_OUT = 2
} SHT1x_Result_t;

/**
 * @brief  Control Heater data type
 */
typedef enum SHT1x_Heater_e
{
  SHT1x_HeaterOff = 0,
  SHT1x_HeaterOn = 1
} SHT1x_Heater_t;


/* Functionality Options --------------------------------------------------------*/
/**
 * @brief  Fahrenheit measurement option
 *         - 0: Disable Fahrenheit measurement function
 *         - 1: Enable Fahrenheit measurement function
 */
#define SHT1x_FahrenheitMeasurement 0

/**
 * @brief  High resolution measurement option
 * @note   By setting this constant to 0, high resolution is selected
 *         - 0: Disable High resolution measurement
 *         - 1: Enable High resolution measurement
 */
#define SHT1x_ResolutionControl 0

/**
 * @brief  Determine how to measure the supply voltage
 *         - 0: The power supply voltage determine by SHT1x_SetPowVoltage function
 *         - 1: The power supply voltage is constant at 3.3 volts
 *         - 2: The power supply voltage is constant at 5 volts
 */
#define SHT1x_PowerVoltageControl 0

/**
 * @brief  Internal heater control option
 *         - 0: Disable internal heater control
 *         - 1: Enable internal heater control
 */
#define SHT1x_InternalHeaterControl 0



/**
 ==================================================================================
                        ##### Measurement Functions #####                          
 ==================================================================================
 */

/**
 * @brief  Readout of Measurement Results
 * @param  Handler: Pointer to handler
 * @param  Sample: Pointer to sample structure
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 *         - SHT1x_FAIL: Operation failed.
 *         - SHT1x_TIME_OUT: Timeout occurred.
 */
SHT1x_Result_t
SHT1x_ReadSample(SHT1x_Handler_t *Handler, SHT1x_Sample_t *Sample);



/**
 ==================================================================================
                    ##### Control and Status Functions #####                       
 ==================================================================================
 */

/**
 * @brief  Initializes platform-independent fields of Handler
 * @param  Handler: Pointer to handler
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 */
SHT1x_Result_t
SHT1x_Init(SHT1x_Handler_t *Handler);


/**
 * @brief  De-initializes the Handler and calls DataDeInit and SckDeInit functions
 * @param  Handler: Pointer to handler
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 */
SHT1x_Result_t
SHT1x_DeInit(SHT1x_Handler_t *Handler);


/**
 * @brief  Resets SHT1x. This takes about 20ms
 * @param  Handler: Pointer to handler
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 *         - SHT1x_FAIL: Operation failed.
 */
SHT1x_Result_t
SHT1x_SoftReset(SHT1x_Handler_t *Handler);


#if (SHT1x_ResolutionControl)
/**
 * @brief  Set Resolution of measurement.
 * @param  Handler: Pointer to handler
 * @param  Resolution: Measurement Resolution
 *         - SHT1x_LowResolution: Temperature 12-bit & Humidity 8-bit.
 *         - SHT1x_HighResolution: Temperature 14-bit & Humidity 12-bit.
 * 
 * @note   Low resolution mode is especially useful in high speed or extreme low
 *         power applications.
 * 
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 *         - SHT1x_FAIL: Operation failed.
 */
SHT1x_Result_t
SHT1x_SetResolution(SHT1x_Handler_t *Handler, SHT1x_Resolution_t Resolution);


/**
 * @brief  Get Resolution of measurement.
 * @param  Handler: Pointer to handler
 * @param  Resolution: Pointer to get measurement mode
 *         - SHT1x_LowResolution: Temperature 12-bit & Humidity 8-bit.
 *         - SHT1x_HighResolution: Temperature 14-bit & Humidity 12-bit.
 * 
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 *         - SHT1x_FAIL: Operation failed.
 */
SHT1x_Result_t
SHT1x_GetResolution(SHT1x_Handler_t *Handler, SHT1x_Resolution_t *Resolution);
#endif


#if (SHT1x_InternalHeaterControl)
/**
 * @brief  Set internal heater on or off.
 * @param  Handler: Pointer to handler
 * @param  Heater:
 *         - SHT1x_HeaterOff: Turn off internal heater.
 *         - SHT1x_HeaterOn: Turn on internal heater.
 * 
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 *         - SHT1x_FAIL: Operation failed.
 */
SHT1x_Result_t
SHT1x_SetInternalHeater(SHT1x_Handler_t *Handler, SHT1x_Heater_t Heater);


/**
 * @brief  Get internal heater state.
 * @param  Handler: Pointer to handler
 * @param  Heater: Pointer to give internal heater state.
 *         - SHT1x_HeaterOff: Turn off internal heater.
 *         - SHT1x_HeaterOn: Turn on internal heater.
 * 
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 *         - SHT1x_FAIL: Operation failed.
 */
SHT1x_Result_t
SHT1x_GetInternalHeater(SHT1x_Handler_t *Handler, SHT1x_Heater_t *Heater);
#endif


#if (SHT1x_PowerVoltageControl == 0)
/**
 * @brief  Set The SHT1x power supply voltage.
 * @param  Handler: Pointer to handler
 * @param  Voltage: Sht1x power supply voltage.
 * @note   The accuracy of temperature measurement is related to the accuracy of the
 *         input voltage.
 * @note   The default power supply voltage is 5V. 
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 */
SHT1x_Result_t
SHT1x_SetPowVoltage(SHT1x_Handler_t *Handler, float Voltage);
#endif



#ifdef __cplusplus
}
#endif

#endif //! _SHT1X_H
