/**
 ******************************************************************************
 * @author: ThanNT
 * @date:   05/09/2016
 ******************************************************************************
**/

#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "ak.h"
#include "timer.h"
#include "task.h"
#include "message.h"

#define TIMER_TICK					(0x01)

#define TIMER_MSG_NULL				((ak_timer_t*)0)
#define TIMER_RET_OK				(1)
#define TIMER_RET_NG				(0)

/* sizeof timer pool */
#ifndef AK_TIMER_POOL_SIZE
#define AK_TIMER_POOL_SIZE			(16)
#endif

typedef uint8_t						timer_sig_t;

typedef enum {
	TIMER_ONE_SHOT,
	TIMER_PERIODIC
} timer_type_t;

typedef struct ak_timer_t {
	struct ak_timer_t*	next;			/* manager timer message */

	task_id_t			des_task_id;	/* destination task id */
	timer_sig_t			sig;			/* signal for application */

	int32_t				counter;		/* decrease each timer stick (must be sign) */
	int32_t				period;			/* case one-shot timer, this field is equa 0 */
} ak_timer_t;

extern void timer_init();
extern void timer_tick(int32_t t);
extern void task_timer_tick(ak_msg_t* msg);

extern uint8_t timer_set(task_id_t des_task_id, timer_sig_t sig, int32_t duty, timer_type_t type);
extern uint8_t timer_remove_attr(task_id_t des_task_id, timer_sig_t sig);

#ifdef __cplusplus
}
#endif

#endif //__TIMER_H__
