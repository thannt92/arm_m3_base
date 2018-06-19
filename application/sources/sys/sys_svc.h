#ifndef __SYS_SVC_H__
#define __SYS_SVC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

extern void sys_svc_restart_app();
extern void sys_svc_unprivileged_state();

#ifdef __cplusplus
}
#endif

#endif // __SYS_SVC_H__
