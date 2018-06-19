#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_io.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_if.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_list_if.h"
#include "task_dbg.h"

#include "link_sig.h"
#include "link.h"

void task_dbg(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_DBG_TEST_1: {
		APP_DBG_SIG("AC_DBG_TEST_1\n");
		uint8_t test_buf[64];
		for (int i = 0; i < 64; i++) {
			test_buf[i] = 0xAA;
		}

		ak_msg_t* s_msg = get_common_msg();
		set_if_des_type(s_msg, IF_TYPE_UART_GW);
		set_if_src_type(s_msg, IF_TYPE_UART_AC);
		set_if_des_task_id(s_msg, GW_TASK_DEBUG_MSG_ID);
		set_if_sig(s_msg, GW_DEBUG_MSG_2);
		set_if_data_common_msg(s_msg, test_buf, 64);

		set_msg_sig(s_msg, AC_LINK_SEND_COMMON_MSG);
		task_post(AC_LINK_ID, s_msg);
	}
		break;

	case AC_DBG_TEST_2: {
		APP_DBG_SIG("AC_DBG_TEST_2\n");
		uint8_t test_buf[256];
		for (int i = 0; i < 256; i++) {
			test_buf[i] = 0xAA;
		}

		ak_msg_t* s_msg = get_dynamic_msg();
		set_if_des_type(s_msg, IF_TYPE_UART_GW);
		set_if_src_type(s_msg, IF_TYPE_UART_AC);
		set_if_des_task_id(s_msg, GW_TASK_DEBUG_MSG_ID);
		set_if_sig(s_msg, GW_DEBUG_MSG_6);
		set_if_data_dynamic_msg(s_msg, test_buf, 256);

		set_msg_sig(s_msg, AC_LINK_SEND_DYNAMIC_MSG);
		task_post(AC_LINK_ID, s_msg);
	}
		break;

	case AC_DBG_TEST_3: {
		APP_DBG_SIG("AC_DBG_TEST_3\n");
	}
		break;

	case AC_DBG_TEST_4: {
		APP_DBG_SIG("AC_DBG_TEST_43\n");
		timer_set(AC_TASK_DBG_ID, AC_DBG_TEST_5, 10, TIMER_ONE_SHOT);
	}
		break;

	case AC_DBG_TEST_5: {
		APP_DBG_SIG("AC_DBG_TEST_5\n");
		ak_msg_t* s_msg = get_dynamic_msg();
		uint8_t* send_data = (uint8_t*)ak_malloc(254);
		for (uint8_t i = 0; i < 254; i++) {
			*(send_data + i) = i;
		}
		set_data_dynamic_msg(s_msg, send_data, 254);
		set_msg_sig(s_msg, AC_DBG_TEST_6);
		task_post(AC_TASK_DBG_ID, s_msg);
		ak_free(send_data);
	}
		break;

	case AC_DBG_TEST_6: {
		APP_DBG_SIG("AC_DBG_TEST_6: %d\n", get_data_len_dynamic_msg(msg));
	}
		break;

	case AC_DBG_TEST_7: {
		APP_DBG_SIG("AC_DBG_TEST_7\n");
		sys_ctrl_delay_ms(240);
		timer_set(AC_TASK_DBG_ID, AC_DBG_TEST_7, 1000, TIMER_ONE_SHOT);
	}
		break;

	default:
		break;
	}
}
