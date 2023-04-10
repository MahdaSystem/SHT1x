/**
 **********************************************************************************
 * @file   SHT1x.c
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

/* Includes ---------------------------------------------------------------------*/
#include "SHT1x.h"


/* Private Constants ------------------------------------------------------------*/
#define SHT1x_CMD_MeasureTemperature  0x03
#define SHT1x_CMD_MeasureHumidity     0x05
#define SHT1x_CMD_ReadStatusRegister  0x07
#define SHT1x_CMD_WriteStatusRegister 0x06
#define SHT1x_CMD_SoftReset           0x1E



/**
 ==================================================================================
                            ##### static functions #####                           
 ==================================================================================
 */

static inline void
SHT1x_Start(SHT1x_Handler_t *Handler)
{
  /*
   * SHT11
   * Start Sequence
   *        __    __
   * SCK  _|  |__|  |__
   *      __       ____
   * DATA   |_____|
   */

  Handler->DataWriteHigh();
  Handler->DelayUs(2);

  Handler->SckWriteHigh();
  Handler->DelayUs(2);

  Handler->DataWriteLow();
  Handler->DelayUs(2);

  Handler->SckWriteLow();
  Handler->DelayUs(8);

  Handler->SckWriteHigh();
  Handler->DelayUs(2);

  Handler->DataWriteHigh();
  Handler->DelayUs(2);

  Handler->SckWriteLow();
}

static inline void
SHT1x_SendACK(SHT1x_Handler_t *Handler)
{
  Handler->DataConfigOut();

  Handler->DataWriteLow();
  Handler->DelayUs(4);
  Handler->SckWriteHigh();
  Handler->DelayUs(4);
  Handler->SckWriteLow();
  Handler->DelayUs(4);
}

static inline void
SHT1x_SiftIn(SHT1x_Handler_t *Handler, uint8_t *Data)
{
  uint8_t DataBuff = 0;

  for (int8_t counter = 7; counter >= 0; --counter)
  {
    Handler->SckWriteHigh();
    Handler->DelayUs(4);
    DataBuff |= (Handler->DataRead() << counter);
    Handler->SckWriteLow();
    Handler->DelayUs(4);
  }

  *Data = DataBuff;
}

//Read measurement from sensor
static void
SHT1x_shiftDataIn(SHT1x_Handler_t *Handler, uint16_t *Data)
{
  uint16_t val1 = 0;
  uint8_t read1 = 0;

  Handler->DataConfigIn();

  SHT1x_SiftIn(Handler, &read1); // read MSB byte

  val1 = (read1 << 8); //read1*256;

  //Send acknowledgment to sensor that MSB byte is read
  SHT1x_SendACK(Handler);

  Handler->DataConfigIn();

  //read LSB byte of from the sensor
  SHT1x_SiftIn(Handler, &read1);
  val1 |= read1;

  //return the read value from sensor
  *Data = val1;
}

//Send the command to read temp or humidity to micro controller
static SHT1x_Result_t
SHT1x_SendCmd(SHT1x_Handler_t *Handler, uint8_t CMD)
{
  Handler->DataConfigOut();
  Handler->SckConfigOut();

  //Initiate the start signal to sensor
  SHT1x_Start(Handler);

  // send the command to sensor
  for (uint8_t counter = 0; counter < 8; counter++, CMD <<= 1)
  {
    if (CMD & 0x80)
      Handler->DataWriteHigh();
    else
      Handler->DataWriteLow();

    Handler->DelayUs(4);
    Handler->SckWriteHigh();

    Handler->DelayUs(4);
    Handler->SckWriteLow();
  }

  Handler->DataConfigIn();

  //Check acknowledgments if the sensor has ack the cmd
  if (Handler->DataRead())
    return SHT1x_FAIL;

  Handler->DelayUs(4);
  Handler->SckWriteHigh();
  Handler->DelayUs(4);
  Handler->SckWriteLow();

  return SHT1x_OK;
}

// pooling for the sensor to complete measuring data
static SHT1x_Result_t
SHT1x_WaitForResult(SHT1x_Handler_t *Handler)
{
  uint8_t ack = 0;

  Handler->DataConfigIn();

  for (uint16_t counter = 0; counter < 50; counter++)
  {
    ack = Handler->DataRead();
    if (!ack)
      return SHT1x_OK;

    Handler->DelayMs(10);
  }

  return SHT1x_TIME_OUT;
}

//Skip the CRC
static SHT1x_Result_t
SHT1x_CheckCRC(SHT1x_Handler_t *Handler)
{
  Handler->DataConfigOut();
  Handler->DataWriteHigh();

  for (uint8_t counter = 0; counter < 9; counter++)
  {
    Handler->DelayUs(4);
    Handler->SckWriteHigh();
    Handler->DelayUs(4);
    Handler->SckWriteLow();
  }

  SHT1x_Start(Handler);

  return SHT1x_OK;
}

static SHT1x_Result_t
SHT1x_ReadStatusRegister(SHT1x_Handler_t *Handler, uint8_t *Reg)
{
  //Send command to read Status Register
  if (SHT1x_SendCmd(Handler, SHT1x_CMD_ReadStatusRegister) != SHT1x_OK)
    return SHT1x_FAIL;

  SHT1x_SiftIn(Handler, Reg);

  SHT1x_CheckCRC(Handler);

  return SHT1x_OK;
}

static SHT1x_Result_t
SHT1x_WriteStatusRegister(SHT1x_Handler_t *Handler, uint8_t Reg)
{
  //Send command to Write Status Register
  if (SHT1x_SendCmd(Handler, SHT1x_CMD_WriteStatusRegister) != SHT1x_OK)
    return SHT1x_FAIL;

  Handler->DataConfigOut();

  for (uint8_t counter = 0; counter < 8; counter++, Reg <<= 1)
  {
    if (Reg & 0x80)
      Handler->DataWriteHigh();
    else
      Handler->DataWriteLow();

    Handler->DelayUs(4);
    Handler->SckWriteHigh();

    Handler->DelayUs(4);
    Handler->SckWriteLow();
  }

  Handler->DataConfigIn();

  //Check acknowledgments if the sensor has ack the cmd
  if (Handler->DataRead())
    return SHT1x_FAIL;

  Handler->DelayUs(4);
  Handler->SckWriteHigh();
  Handler->DelayUs(4);
  Handler->SckWriteLow();

  return SHT1x_OK;
}

static SHT1x_Result_t
SHT1x_ReadTemp(SHT1x_Handler_t *Handler, uint16_t *TempRaw)
{
  if (SHT1x_SendCmd(Handler, SHT1x_CMD_MeasureTemperature) != SHT1x_OK)
    return SHT1x_FAIL;

  Handler->DataConfigIn();

  //check if sensor has started measuring data after ack
  if (!Handler->DataRead())
    return SHT1x_FAIL;

  //wait till sensor finishes reading data
  if (SHT1x_WaitForResult(Handler) != SHT1x_OK)
    return SHT1x_TIME_OUT;

  //read the data from the Sensor
  SHT1x_shiftDataIn(Handler, TempRaw);
  SHT1x_CheckCRC(Handler);

  return SHT1x_OK;
}

static SHT1x_Result_t
SHT1x_ReadHumidity(SHT1x_Handler_t *Handler, uint16_t *HumidityRaw)
{
  Handler->DataConfigIn();
  Handler->SckConfigOut();

  // send cmd to read humidity to sensor
  if (SHT1x_SendCmd(Handler, SHT1x_CMD_MeasureHumidity) != SHT1x_OK)
    return SHT1x_FAIL;

  Handler->DataConfigIn();

  //check if sensor has started measuring data after ack
  if (!Handler->DataRead())
    return SHT1x_FAIL;

  //poll until sensor has finished measuring data
  if (SHT1x_WaitForResult(Handler) != SHT1x_OK)
    return SHT1x_TIME_OUT;

  //read the data from the Sensor
  SHT1x_shiftDataIn(Handler, HumidityRaw);
  SHT1x_CheckCRC(Handler);

  return SHT1x_OK;
}

static float
SHT1x_TempConvertRawC(SHT1x_Handler_t *Handler, uint16_t RawTemp)
{
  const float D1 = Handler->D1Celsius;
  float D2 = 0.01;

  switch (Handler->ResolutionStatus)
  {
  case SHT1x_LowResolution:
    //Temperature constant for sht11 at 12bit
    D2 = 0.04;
    break;

  case SHT1x_HighResolution:
    //Temperature constants for sht11 at 14bit
    D2 = 0.01;
    break;
  }

  //calculation for temperature given in data sheet
  return D1 + (D2 * RawTemp);
}

#if (SHT1x_FahrenheitMeasurement)
/**
 * @brief  Convert raw temperature to Fahrenheit. 
 * @param  Handler: Pointer to handler
 * @param  RawTemp: Raw temperature
 * @retval Temperature in Fahrenheit
 */
static float
SHT1x_TempConvertRawF(SHT1x_Handler_t *Handler, uint16_t RawTemp)
{
  uint16_t obtVal;
  SHT1x_Result_t Result;
  const float D1 = Handler->D1Fahrenheit;
  float D2 = 0.018;

  switch (ResolutionStatus)
  {
  case SHT1x_LowResolution:
    //Temperature constant for sht11 at 12bit
    D2 = 0.072; //for 12 bits
    break;

  case SHT1x_HighResolution:
    //Temperature constants for sht11 at 14bit
    D2 = 0.018; //for 14 bits
    break;
  }

  //calculation for temperature given in data sheet
  return D1 + (D2 * RawTemp);
}
#endif

static float
SHT1x_HumConvertRawP(SHT1x_Handler_t *Handler, uint16_t RawHum, float TempC)
{
  float realHumidity;

  const float c1 = -4;
  float c2 = 0.0405;
  float c3 = -0.0000028;
  const float t1 = 0.01;
  float t2 = t2 = 0.00008;

  switch (Handler->ResolutionStatus)
  {
  case SHT1x_LowResolution:
    //Humidity constants for 8 bit
    c2 = 0.648;
    c3 = -0.00072;

    //Temperature constants for 8 bit
    t2 = 0.00128;
    break;

  case SHT1x_HighResolution:
    //Humidity constants for 12 bit
    c2 = 0.0405;
    c3 = -0.0000028;

    //Temperature constants for 12 bit
    t2 = 0.00008;
    break;
  }

  //convert it into relative humidity for linear compensation
  realHumidity = c1 + (c2 * RawHum) + (c3 * RawHum * RawHum);
  // calculate relative humidity with temperature compensation
  return (TempC - 25.0) * (t1 + (t2 * RawHum)) + realHumidity;
}



/**
 ==================================================================================
                      ##### Public Measurement Functions #####                     
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
SHT1x_ReadSample(SHT1x_Handler_t *Handler, SHT1x_Sample_t *Sample)
{
  uint16_t Buffer;
  SHT1x_Result_t Result;

  //get the sensor reading raw data for humidity
  Result = SHT1x_ReadHumidity(Handler, &Buffer);
  if (Result != SHT1x_OK)
    return Result;
  Sample->HumRaw = Buffer;

  //get the sensor reading raw data for temperature
  Result = SHT1x_ReadTemp(Handler, &Buffer);
  if (Result != SHT1x_OK)
    return Result;
  Sample->TempRaw = Buffer;

  Sample->TempCelsius = SHT1x_TempConvertRawC(Handler, Sample->TempRaw);
#if (SHT1x_FahrenheitMeasurement)
  Sample->TempFahrenheit = SHT1x_TempConvertRawF(Handler, Sample->TempRaw);
#endif
  Sample->HumidityPercent = SHT1x_HumConvertRawP(Handler, Sample->HumRaw,
                                                 Sample->TempCelsius);

  return SHT1x_OK;
}



/**
 ==================================================================================
                        ##### Public Control Functions #####                       
 ==================================================================================
 */

/**
 * @brief  Initializes platform-independent fields of Handler
 * @param  Handler: Pointer to handler
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 */
SHT1x_Result_t
SHT1x_Init(SHT1x_Handler_t *Handler)
{
#if (SHT1x_PowerVoltageValue == 0 || SHT1x_PowerVoltageValue == 2)
  Handler->D1Celsius = -40;
  Handler->D1Fahrenheit = -40;
#elif (SHT1x_PowerVoltageValue == 1)
  Handler->D1Celsius = -39.63;
  Handler->D1Fahrenheit = -39.31;
#endif

  Handler->ResolutionStatus = SHT1x_HighResolution;

  return SHT1x_OK;
}


/**
 * @brief  De-initializes the Handler and calls DataDeInit and SckDeInit functions
 * @param  Handler: Pointer to handler
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 */
SHT1x_Result_t
SHT1x_DeInit(SHT1x_Handler_t *Handler)
{
  Handler->DataDeInit();
  Handler->SckDeInit();

  return SHT1x_OK;
}


/**
 * @brief  Resets SHT1x. This takes about 20ms
 * @param  Handler: Pointer to handler
 * @retval SHT1x_Result_t
 *         - SHT1x_OK: Operation was successful.
 *         - SHT1x_FAIL: Operation failed.
 */
SHT1x_Result_t
SHT1x_SoftReset(SHT1x_Handler_t *Handler)
{
  //Send command to soft reset.
  if (SHT1x_SendCmd(Handler, SHT1x_CMD_SoftReset) != SHT1x_OK)
    return SHT1x_FAIL;

#if (SHT1x_ResolutionControl)
  Handler->ResolutionStatus = SHT1x_HighResolution;
#endif

  Handler->DelayMs(20);

  return SHT1x_OK;
}


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
SHT1x_SetResolution(SHT1x_Handler_t *Handler, SHT1x_Resolution_t Resolution)
{
  uint8_t StatusReg = 0;

  if (SHT1x_ReadStatusRegister(Handler, &StatusReg) != SHT1x_OK)
    return SHT1x_FAIL;

  switch (Resolution)
  {
  case SHT1x_LowResolution:
    StatusReg |= 0x01;
    break;

  case SHT1x_HighResolution:
    StatusReg &= 0xFE;
    break;
  }

  if (SHT1x_WriteStatusRegister(Handler, StatusReg) != SHT1x_OK)
    return SHT1x_FAIL;

  Handler->ResolutionStatus = Resolution;

  return SHT1x_OK;
}
#endif


#if (SHT1x_ResolutionControl)
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
SHT1x_GetResolution(SHT1x_Handler_t *Handler, SHT1x_Resolution_t *Resolution)
{
  uint8_t StatusReg = 0;

  if (SHT1x_ReadStatusRegister(Handler, &StatusReg) != SHT1x_OK)
    return SHT1x_FAIL;

  if (StatusReg & 0x01)
    *Resolution = SHT1x_LowResolution;
  else
    *Resolution = SHT1x_HighResolution;

  Handler->ResolutionStatus = *Resolution;

  return SHT1x_OK;
}
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
SHT1x_SetInternalHeater(SHT1x_Handler_t *Handler, SHT1x_Heater_t Heater)
{
  uint8_t StatusReg = 0;

  if (SHT1x_ReadStatusRegister(Handler, &StatusReg) != SHT1x_OK)
    return SHT1x_FAIL;

  switch (Heater)
  {
  case SHT1x_HeaterOff:
    StatusReg &= 0xFB;
    break;

  case SHT1x_HeaterOn:
    StatusReg |= 0x04;
    break;
  }

  if (SHT1x_WriteStatusRegister(Handler, StatusReg) != SHT1x_OK)
    return SHT1x_FAIL;

  return SHT1x_OK;
}
#endif


#if (SHT1x_InternalHeaterControl)
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
SHT1x_GetInternalHeater(SHT1x_Handler_t *Handler, SHT1x_Heater_t *Heater)
{
  uint8_t StatusReg = 0;

  if (SHT1x_ReadStatusRegister(Handler, &StatusReg) != SHT1x_OK)
    return SHT1x_FAIL;

  if (StatusReg & 0x04)
    *Heater = SHT1x_HeaterOn;
  else
    *Heater = SHT1x_HeaterOff;

  return SHT1x_OK;
}
#endif


#if (SHT1x_PowerVoltageControl)
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
SHT1x_SetPowVoltage(SHT1x_Handler_t *Handler, float Voltage)
{
  Handler->D1Celsius = (-0.0462 * Voltage * Voltage) + (0.1672 * Voltage) - (39.682);

#if (SHT1x_FahrenheitMeasurement)
  Handler->D1Fahrenheit = (-0.1249 * Voltage * Voltage) + (0.633 * Voltage) - (40.039);
#endif

  return SHT1x_OK;
}
#endif
