/**
 ******************************************************************************
 * @Author: ThanNT
 * @Date:   05/09/2016
 ******************************************************************************
**/
#include "sys_dbg.h"
#include "sys_ctrl.h"

#include "../driver/eeprom/eeprom.h"
#include "../driver/flash/flash.h"

#include "../app/app_eeprom.h"
#include "../app/app_flash.h"

#include "../common/utils.h"

#if defined(STM32L_PLATFORM_BOOT)
#include "../common/xprintf.h"
#include "../platform/stm32l/io_cfg.h"
#include "../platform/stm32l/sys_cfg.h"
#else
#error Please choose platform for kernel.
#endif

void sys_dbg_fatal(const int8_t* s, uint8_t c) {
	xprintf("%s\t%x\n", s, c);
	sys_ctrl_reset();
}
