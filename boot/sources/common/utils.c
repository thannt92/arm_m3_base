/**
 ******************************************************************************
 * @Author: ThanNT
 * @Date:   13/08/2016
 ******************************************************************************
**/

#include "utils.h"
#include <stdint.h>

uint32_t str_len(const int8_t *str) {
	uint8_t length = 0;
	while (str[length] != 0) {
		length++;
	}
	return length;
}

int8_t* itoa(int32_t num, int8_t* str, uint32_t base) {
	uint8_t i = 0;
	int8_t isNegative = 0;
	uint8_t start, end, tmp;
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	if (num < 0 && base == 10) {
		isNegative = 1;
		num = -num;
	}

	while (num != 0) {
		int rem = num % base;
		str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
		num = num/base;
	}

	if (isNegative)
		str[i++] = '-';

	str[i] = '\0';

	start = 0;
	end = i - 1;
	while(start < end) {
		tmp = str[start];
		str[start] = str[end];
		str[end] = tmp;
		start++;
		end--;
	}

	return str;
}

void *mem_set(void *str, int c,size_t size) {
	uint8_t *ptr = (uint8_t *)str;
	const uint8_t ch = c;
	while(size--)
		*ptr++ = ch;
	return str;
}

void mem_cpy(void *dst,const void *str, size_t size) {
	uint8_t *ptr = (uint8_t *)str;
	uint8_t *ptr2 = (uint8_t *)dst;
	while(size--)
		*ptr2++ = *ptr++;
}

uint8_t  mem_read(uint32_t address, uint8_t* data, uint32_t len) {
	uint8_t *src = (uint8_t *)address;
	uint8_t *dat = (uint8_t *)data;

	while(len--) {
		*dat++ = *src++;
	}

	return 0;
}

uint8_t  mem_write(uint32_t address, uint8_t* data, uint32_t len) {
	uint8_t *dst = (uint8_t *)address;
	uint8_t *dat = (uint8_t *)data;

	while(len--) {
		*dst++ = *dat++;
	}

	return 0;
}

int32_t str_cmp(const int8_t *str1, const int8_t *str2) {
	while (*str1 == *str2) {
		if (*str1 == '\0' || *str2 == '\0')
			break;
		str1++;
		str2++;
	}

	if (*str1 == '\0' && *str2 == '\0')
		return 0;
	else
		return -1;
}

int8_t * str_cpy(int8_t * destination, const int8_t * source) {
	uint32_t len = 0;
	uint32_t index = 0;

	if (source == (int8_t*)0) {
		return (int8_t*)0;
	}

	if (destination == (int8_t*)0) {
		return (int8_t*)0;
	}

	len = str_len(source);

	for (index = 0; index <= len; index++) {
		*(destination + index) = *(source + index);
	}

	return destination;
}
