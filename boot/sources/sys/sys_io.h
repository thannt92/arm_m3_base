#ifndef __SYS_IO_H__
#define __SYS_IO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(STM32L_PLATFORM_BOOT)
#include "../platform/stm32l/io_cfg.h"
#include "../platform/stm32l/stm32l.h"
#else
#error Please choose platform for app.
#endif

#ifdef __cplusplus
}
#endif

#endif // __SYS_IO_H__
