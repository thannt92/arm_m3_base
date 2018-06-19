#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "view_render.h"

#include "app.h"
#include "app_dbg.h"

#include "task_list.h"
#include "task_display.h"

scr_mng_t scr_mng_app;

/* list of screen handler */
void scr_startup_handle(ak_msg_t* msg);

/* list of view dynamic function */
static void view_scr_startup();

/**
 ******************************************************************************
 * objects MAIN SCREEN
 *
 ******************************************************************************
 */

/*----------[[[ITERM]]]------------*/

view_dynamic_t dyn_view_startup = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_startup
};

/*----------[[[OBJECT]]]------------*/
view_screen_t scr_startup = {
	&dyn_view_startup,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void task_display(ak_msg_t* msg) {
	scr_mng_dispatch(msg);
}

void scr_startup_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_DISPLAY_INITIAL: {
		APP_DBG_SIG("AC_DISPLAY_INITIAL\n");
		view_render.initialize();
		view_render_display_off();
		SCREEN_NONE_UPDATE_MASK();
		task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_ON_LOGO);
	}
		break;

	case AC_DISPLAY_SHOW_ON_LOGO: {
		APP_DBG_SIG("AC_DISPLAY_SHOW_ON_LOGO\n");
		view_render_display_on();
		timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_OFF, AC_DISPLAY_LOGO_INTERVAL, TIMER_ONE_SHOT);
	}
		break;

	case AC_DISPLAY_SHOW_OFF: {
		APP_DBG_SIG("AC_DISPLAY_SHOW_OFF\n");
		view_render_display_off();
		SCREEN_NONE_UPDATE_MASK();
	}
		break;

	default:
		break;
	}
}

void view_scr_startup() {
	/* ak logo */
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(24, 3);
	view_render.print("   __    _  _ ");
	view_render.setCursor(24, 10);
	view_render.print("  /__\\  ( )/ )");
	view_render.setCursor(24, 20);
	view_render.print(" /(__)\\ (   (");
	view_render.setCursor(24, 30);
	view_render.print("(__)(__)(_)\\_)");
	view_render.setCursor(24, 45);
	view_render.print("Active Kernel");
	view_render.update();
}
