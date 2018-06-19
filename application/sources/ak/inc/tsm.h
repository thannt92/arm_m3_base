/**
 ******************************************************************************
 * @author: ThanNT
 * @date:   12/02/2017
 * @brief:  table state machine.
 ******************************************************************************
**/

#ifndef __TSM_H__
#define __TSM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "ak.h"
#include "message.h"

#define TSM_FUNCTION_NULL		((tsm_func_f)0)
typedef void (*tsm_func_f)(ak_msg_t*);

typedef uint8_t tsm_state_t;

typedef void (*on_tsm_state)(tsm_state_t);

typedef struct {
	uint8_t sig;				/* signal */
	tsm_state_t next_state;		/* next state */
	tsm_func_f tsm_func;		/* state handler function */
} tsm_t;

typedef struct tsm_tbl_t {
	tsm_state_t state;
	on_tsm_state on_state;
	tsm_t** table;
} tsm_tbl_t;

#define TSM(t, tbl, s) tsm_init(t, tbl, s)
#define TSM_TRAN(t, s) tsm_tran(t, s)

void tsm_init(tsm_tbl_t* tsm_tbl, tsm_t** tbl, tsm_state_t state);
void tsm_dispatch(tsm_tbl_t* tsm_tbl, ak_msg_t* msg);
void tsm_tran(tsm_tbl_t* tsm_tbl, tsm_state_t state);

#ifdef __cplusplus
}
#endif

#endif //__TSM_H__
