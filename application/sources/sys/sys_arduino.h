#ifndef __SYS_ARDUINO_H__
#define __SYS_ARDUINO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(STM32L_PLATFORM)
#include "Arduino.h"
#else
#error Please choose platform for app.
#endif

#ifdef __cplusplus
}
#endif

#endif // __SYS_ARDUINO_H__
