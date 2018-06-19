/**
 ******************************************************************************
 * @Author: ThanNT
 * @Date:   13/08/2016
 ******************************************************************************
**/

#ifndef __UTILS_H__
#define __UTILS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <string.h>

extern int8_t* itoa(int32_t num, int8_t* str, uint32_t base);
extern uint32_t str_len(const int8_t *str);

extern void *mem_set(void *str, int c,size_t size);
extern void mem_cpy(void *dst,const void *str, size_t size);
extern uint8_t  mem_read(uint32_t, uint8_t*, uint32_t);
extern uint8_t  mem_write(uint32_t, uint8_t*, uint32_t);
extern int32_t str_cmp(const int8_t *str1, const int8_t *str2);
extern int8_t * str_cpy (int8_t * destination, const int8_t * source);

#ifdef __cplusplus
}
#endif

#endif //__UTILS_H__
