
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */
 
 /* spaniakos <spaniakos@gmail.com>
  Added __ARDUINO_X86__ support
*/

#ifndef __RF24_CONFIG_H__
#define __RF24_CONFIG_H__

#define rf24_max(a,b) (a>b?a:b)
#define rf24_min(a,b) (a<b?a:b)
 
#if defined (STM32L_PLATFORM)
#include <stdint.h>

#include "../platform/stm32l/arduino/SPI/SPI.h"
#include "../platform/stm32l/arduino/Arduino.h"
#include "../platform/stm32l/arduino/SPI/SPI.h"
#include "../platform/stm32l/io_cfg.h"
#include "../app/app_dbg.h"
#include "../common/xprintf.h"
#include "../common/utils.h"

typedef uint16_t prog_uint16_t;
#define PSTR(x) (x)
#define printf_P APP_DBG
#define strlen_P strlen
#define strlen_P strlen
#define memcpy_P memcpy
#define PROGMEM
#define pgm_read_word(p) (*(p))
#define PRIPSTR "%s"
#define pgm_read_byte(p) (*(p))

#if defined(SERIAL_DEBUG)
#define IF_SERIAL_DEBUG(x) ({x;})
#else
#define IF_SERIAL_DEBUG(x) ({/*##x;##*/})
#endif

#define _SPI SPI

#elif defined (STM32F10X_PLATFORM)
#include <stdint.h>

#include "../platform/stm32f10x/arduino/SPI/SPI.h"
#include "../platform/stm32f10x/arduino/Arduino.h"
#include "../platform/stm32f10x/arduino/SPI/SPI.h"
#include "../platform/stm32f10x/io_cfg.h"
#include "../app/app_dbg.h"
#include "../common/xprintf.h"
#include "../common/utils.h"

typedef uint16_t prog_uint16_t;
#define PSTR(x) (x)
#define printf_P
#define printf_P APP_DBG
#define strlen_P strlen
#define strlen_P strlen
#define memcpy_P memcpy
#define PROGMEM
#define pgm_read_word(p) (*(p))
#define PRIPSTR "%s"
#define pgm_read_byte(p) (*(p))

#if defined(SERIAL_DEBUG)
#define IF_SERIAL_DEBUG(x) ({x;})
#else
#define IF_SERIAL_DEBUG(x) ({/*##x;##*/})
#endif

#define _SPI SPI

#endif

#endif // __RF24_CONFIG_H__

