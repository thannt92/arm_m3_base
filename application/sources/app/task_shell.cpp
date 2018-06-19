/**
 ******************************************************************************
 * @author: ThanNT
 * @date:   13/08/2016
 ******************************************************************************
**/

#include <stdbool.h>

#include "fsm.h"
#include "port.h"
#include "message.h"

#include "cmd_line.h"
#include "xprintf.h"

#include "sys_dbg.h"
#include "sys_ctrl.h"
#include "sys_irq.h"
#include "sys_svc.h"
#include "sys_io.h"

#include "app.h"
#include "app_dbg.h"
#include "task_shell.h"
#include "task_uart_if.h"
#include "task_list.h"

#include "link_phy.h"
#include "link_hal.h"

#pragma GCC optimize ("O3")

struct shell_t {
	uint8_t index;
	uint8_t data[SHELL_BUFFER_LENGHT];
};

volatile struct shell_t shell;

void sys_irq_shell() {
	volatile uint8_t c = 0;

	c = sys_ctrl_shell_get_char();

	if (plink_hal_rev_byte(c) == LINK_HAL_IGNORED) {

		if (shell.index < SHELL_BUFFER_LENGHT - 1) {

			if (c == '\r' || c == '\n') { /* linefeed */

				xputchar('\r');
				xputchar('\n');

				shell.data[shell.index] = c;
				shell.data[shell.index + 1] = 0;
				task_post_common_msg(AC_TASK_SHELL_ID, AC_SHELL_LOGIN_CMD, (uint8_t*)&shell.data[0], shell.index + 2);

				shell.index = 0;
			}
			else {

				xputchar(c);

				if (c == 8 && shell.index) { /* backspace */
					shell.index--;
				}
				else {
					shell.data[shell.index++] = c;
				}
			}
		}
		else {
			APP_PRINT("\nerror: cmd too long, cmd size: %d, try again !\n", SHELL_BUFFER_LENGHT);
			shell.index = 0;
		}
	}
}

void task_shell(ak_msg_t* msg) {
	uint8_t fist_char = *(get_data_common_msg(msg));

	switch (msg->sig) {
	case AC_SHELL_LOGIN_CMD:
		set_dymc_output_type(DYMC_UART_SHELL_TYPE);
		break;

	case AC_SHELL_REMOTE_CMD:
		set_dymc_output_type(DYMC_RF_REMOTE_TYPE);
		break;

	default:
		FATAL("SHELL", 0x01);
		break;
	}

	switch (cmd_line_parser(lgn_cmd_table, get_data_common_msg(msg))) {
	case CMD_SUCCESS:
		break;

	case CMD_NOT_FOUND:
		if (fist_char != '\r' &&
				fist_char != '\n') {
			LOGIN_PRINT("cmd unknown\n");
		}
		break;

	case CMD_TOO_LONG:
		LOGIN_PRINT("cmd too long\n");
		break;

	case CMD_TBL_NOT_FOUND:
		LOGIN_PRINT("cmd table not found\n");
		break;

	default:
		LOGIN_PRINT("cmd error\n");
		break;
	}

	LOGIN_PRINT("#");
}
