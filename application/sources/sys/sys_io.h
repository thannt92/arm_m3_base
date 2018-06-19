#ifndef __SYS_IO_H__
#define __SYS_IO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(STM32L_PLATFORM)
#include "io_cfg.h"
#elif defined(STM32F10X_PLATFORM)
#include "io_cfg.h"
#else
#error Please choose platform for app.
#endif

#ifdef __cplusplus
}
#endif

#endif // __SYS_IO_H__
