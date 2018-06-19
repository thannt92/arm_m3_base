#include "sys_svc.h"
#include "sys_io.h"

void sys_svc_restart_app() {
	__asm volatile ("svc #0x01");
}

void sys_svc_unprivileged_state() {
	__asm volatile ("svc #0x02");
}
