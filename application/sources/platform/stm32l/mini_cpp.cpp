#include <cstdlib>
#include <sys/types.h>
#include "sys_dbg.h"
#include "message.h"

/*
 * The default pulls in 70K of garbage
 */
namespace __gnu_cxx {

void __verbose_terminate_handler() {
	for (;;);
}

}

/*
 * The default pulls in about 12K of garbage
 */
extern "C" void __cxa_pure_virtual() {
	FATAL("C++", 0x01);
}

extern "C" void __cxa_deleted_virtual() {
	FATAL("C++", 0x02);
}

/*
 * Implement C++ new/delete operators using the heap
 */
void *operator new(size_t size) {
	return ak_malloc(size);
}

void *operator new[](size_t size) {
	return ak_malloc(size);
}

void operator delete(void *p) {
	ak_free(p);
}

void operator delete[](void *p) {
	ak_free(p);
}

/*
 * sbrk function for getting space for malloc and friends
 */

/* start heap region */
extern uint32_t __heap_start__;

extern "C" {
caddr_t _sbrk (uint32_t incr) {
	static uint8_t* heap = NULL;
	uint8_t* prev_heap;

	if (heap == NULL) {
		heap = (uint8_t*)((uint32_t)&__heap_start__);
	}

	prev_heap = heap;
	heap += incr;

	return (caddr_t) prev_heap;
}
} /* extern "C" */
