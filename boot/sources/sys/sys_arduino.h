#ifndef __SYS_ARDUINO_H__
#define __SYS_ARDUINO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(STM32L_PLATFORM_BOOT)
#include "../platform/stm32l/arduino/Arduino.h"
#include "../platform/stm32l/arduino/SPI/SPI.h"
#elif defined(STM32F10X_PLATFORM)
#include "../platform/stm32f10x/arduino/Arduino.h"
#include "../platform/stm32f10x/arduino/SPI/SPI.h"
#else
#error Please choose platform for app.
#endif

#ifdef __cplusplus
}
#endif

#endif // __SYS_ARDUINO_H__
