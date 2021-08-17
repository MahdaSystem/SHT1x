# SHT1x Library
SHT1x series sensor driver.
- Read Temperature in Raw data, Celsius and Fahrenheit
- Read Humidity in Raw data and percentage
- Config sensor resolution
- Control internal heater

## Hardware Support
It is easy to port this library to any platform. But now it is ready for use in:
- AVR (ATmega32)
- STM32 (HAL)
- ESP32 (esp-idf)

## How To Use
1. Add `SHT1x.h` and `SHT1x.c` files to your project.  It is optional to use `SHT1x_platform.h` and `SHT1x_platform.c` files (open and config `SHT1x_platform.h` file).
2. Initialize platform-dependent part of handler.
4. Call `SHT1x_Init()`.
5. Call other functions and enjoy.

## Example
<details>
<summary>Using SHT1x_platform files</summary>

```c
#include <stdio.h>
#include "SHT1x.h"
#include "SHT1x_platform.h"

int main(void)
{
  SHT1x_Handler_t Handler;
  SHT1x_Sample_t  Sample;

  SHT1x_Platform_Init(&Handler);
  SHT1x_Init(&Handler);

  while (1)
  {
    SHT1x_ReadSample(&Handler, &Sample);
    printf("Temperature: %f°C\r\n"
           "Humidity: %f%%\r\n\r\n",
           Sample.TempCelsius,
           Sample.HumidityPercent);
  }

  SHT1x_DeInit(&Handler);
  return 0;
}
```
</details>


<details>
<summary>Without using SHT1x_platform files (AVR)</summary>

```c
#include <stdio.h>
#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include "SHT1x.h"

#define DATA_DDR  DDRA
#define DATA_PORT PORTA
#define DATA_PIN  PINA
#define DATA_NUM  0

#define SCK_DDR   DDRA
#define SCK_PORT  PORTA
#define SCK_NUM   1


void SHT1x_Platform_DataDeInit(void)
{
  DATA_PORT &= ~(1<<DATA_NUM);
  DATA_DDR &= ~(1<<DATA_NUM);
}
void SHT1x_Platform_DataConfigOut(void)
{
  DATA_DDR |= (1<<DATA_NUM);
}
void SHT1x_Platform_DataConfigIn(void)
{
  DATA_DDR &= ~(1<<DATA_NUM);
}
void SHT1x_Platform_DataWriteHigh(void)
{
  DATA_PORT |= (1<<DATA_NUM);
}
void SHT1x_Platform_DataWriteLow(void)
{
  DATA_PORT &= ~(1<<DATA_NUM);
}
uint8_t SHT1x_Platform_DataRead(void)
{
  return (DATA_PIN >> DATA_NUM) & 0x01;
}


void SHT1x_Platform_SckDeInit(void)
{
  SCK_PORT &= ~(1<<SCK_NUM);
  SCK_DDR &= ~(1<<SCK_NUM);
}
void SHT1x_Platform_SckConfigOut(void)
{
  SCK_DDR |= (1<<SCK_NUM);
}
void SHT1x_Platform_SckWriteHigh(void)
{
  SCK_PORT |= (1<<SCK_NUM);
}
void SHT1x_Platform_SckWriteLow(void)
{
  SCK_PORT &= ~(1<<SCK_NUM);
}


void SHT1x_Platform_DelayMs(uint8_t Delay)
{
  for (; Delay; --Delay)
    _delay_ms(1);
}
void SHT1x_Platform_DelayUs(uint8_t Delay)
{
  for (; Delay; --Delay)
    _delay_us(1);
}


int main(void)
{
  SHT1x_Handler_t Handler;
  SHT1x_Sample_t  Sample;

  Handler.DataDeInit    = SHT1x_Platform_DataDeInit;
  Handler.DataConfigOut = SHT1x_Platform_DataConfigOut;
  Handler.DataConfigIn  = SHT1x_Platform_DataConfigIn;
  Handler.DataWriteHigh = SHT1x_Platform_DataWriteHigh;
  Handler.DataWriteLow  = SHT1x_Platform_DataWriteLow;
  Handler.DataRead      = SHT1x_Platform_DataRead;
  Handler.SckDeInit     = SHT1x_Platform_SckDeInit;
  Handler.SckConfigOut  = SHT1x_Platform_SckConfigOut;
  Handler.SckWriteHigh  = SHT1x_Platform_SckWriteHigh;
  Handler.SckWriteLow   = SHT1x_Platform_SckWriteLow;
  Handler.DelayMs       = SHT1x_Platform_DelayMs;
  Handler.DelayUs       = SHT1x_Platform_DelayUs;

  SHT1x_Init(&Handler);

  while (1)
  {
    SHT1x_ReadSample(&Handler, &Sample);
    printf("Temperature: %f°C\r\n"
           "Humidity: %f%%\r\n\r\n",
           Sample.TempCelsius,
           Sample.HumidityPercent);
  }

  SHT1x_DeInit(&Handler);
  return 0;
}
```
</details>
