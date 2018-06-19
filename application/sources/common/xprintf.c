/**
 ******************************************************************************
 * @author: ThanNT
 * @date:   13/08/2016
 ******************************************************************************
**/
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "xprintf.h"

#define CR_CRLF								(1)

#define DYMC_BUFFER_SIZE					(256)

#define DYMC_UART_SHELL_TYPE				(0x00)
#define DYMC_RF_REMOTE_TYPE					(0x01)

/* pointer to the output  stream (external) */
void (*xprintf_stream_io_out)(uint8_t);

static uint8_t dymc_print_type = DYMC_UART_SHELL_TYPE;
static uint8_t dymc_print_buf[DYMC_BUFFER_SIZE];

void xputc(x_stream_t* s, uint8_t c) {
	(void)s;
	if (CR_CRLF && (c == (uint8_t)'\n')) {
		xprintf_stream_io_out('\r');
	}
	if (xprintf_stream_io_out) {
		xprintf_stream_io_out(c);
	}
}

void sxputc(x_stream_t* s, uint8_t c) {
	*s->outstr++ = c;
	s->indexstr++;
}

void xputchar(uint8_t c) {
	xprintf_stream_io_out(c);
}

int xstream(x_stream_t* stream, const char *fmt, va_list va_args) {
	uint32_t num, digit;
	int32_t i;

	uint32_t zero_padding = 0;
	uint32_t format_lenght = 0;
	uint32_t base;
	uint32_t minus;
	uint8_t num_stack[11];
	uint8_t* ps;

	while (*fmt) {
		switch (*fmt) {
		case '%':
			zero_padding = 0;
			if (fmt[1] == '0') {
				zero_padding = 1;
				++fmt;
			}
			format_lenght = 0;
			while (*++fmt) {
				switch (*fmt) {
				case '%':
					stream->xputc(stream, *fmt);
					goto next_loop;

				case 'c':
					stream->xputc(stream, va_arg(va_args, int32_t));
					goto next_loop;

				case 'd':
				case 'X':
				case 'x':
					minus = 0;
					num = va_arg(va_args, uint32_t);
					if (*fmt == 'd') {
						base = 10;
						if (num & (uint32_t)0x80000000) {
							num = -(int32_t)num;
							minus = 1;
						}
					} else {
						base = 16;
					}
					for (digit = 0; digit < sizeof(num_stack);) {
						num_stack[digit++] = num%base;
						num /= base;
						if (num == 0) break;
					}
					if (minus) num_stack[digit++] = 0x7F;
					if (format_lenght > digit) {
						int8_t paddingint8_t = ' ';
						format_lenght -= digit;
						if (zero_padding)
							paddingint8_t = '0';
						while (format_lenght--) {
							stream->xputc(stream, paddingint8_t);
						}
					}
					for (i = digit-1; i >= 0; i--) {
						if (num_stack[i] == 0x7F) {
							stream->xputc(stream, '-');
						} else if (num_stack[i] > 9) {
							stream->xputc(stream, num_stack[i]-10 + 'A');
						} else {
							stream->xputc(stream, num_stack[i] + '0');
						}
					}
					goto next_loop;

				case 's':
					ps = va_arg(va_args, uint8_t*);
					while (*ps) {
						stream->xputc(stream, *ps++);
					}
					goto next_loop;

				default:
					if (*fmt >= '0' && *fmt <= '9') {
						format_lenght = format_lenght*10 + (*fmt-'0');
					} else {
						goto exit;
					}
				}
			}

			if (*fmt == 0) {
				goto exit;
			}

			break;

		default:
			stream->xputc(stream, *fmt);
			break;
		}
next_loop:
		fmt++;
	}
exit:
	va_end(va_args);
	return stream->indexstr;
}

void xprintf(const char *fmt, ...) {
	x_stream_t xprintf_stream;
	va_list va;

	va_start(va,fmt);
	xprintf_stream.xputc = xputc;
	xstream(&xprintf_stream, fmt, va);
	va_end(va);
}

int xsprintf(char* str,const char* fmt, ...) {
	x_stream_t xsprintf_stream;
	va_list va;

	va_start(va,fmt);
	xsprintf_stream.indexstr = 0;
	xsprintf_stream.outstr = str;
	xsprintf_stream.xputc = sxputc;
	xstream(&xsprintf_stream, fmt, va);
	va_end(va);

	return xsprintf_stream.indexstr;
}

int set_dymc_output_type(uint8_t type) {
	int ret = -1;
	switch (type) {
	case DYMC_UART_SHELL_TYPE:
	case DYMC_RF_REMOTE_TYPE:
		dymc_print_type = type;
		ret = 0;
		break;
	default:
		break;
	}
	return ret;
}

void xdymcprintf(const char* fmt, ...) {
	x_stream_t xdymcprintf_stream;
	va_list va;

	va_start(va,fmt);

	switch (dymc_print_type) {

	case DYMC_UART_SHELL_TYPE: {
		xdymcprintf_stream.xputc = xputc;
		xstream(&xdymcprintf_stream, fmt, va);
	}
		break;

	case DYMC_RF_REMOTE_TYPE: {
		memset(dymc_print_buf, 0, DYMC_BUFFER_SIZE);
		xdymcprintf_stream.indexstr = 0;
		xdymcprintf_stream.outstr = (char*)dymc_print_buf;
		xdymcprintf_stream.xputc = sxputc;
		xstream(&xdymcprintf_stream, fmt, va);
		rf_printf(dymc_print_buf, xdymcprintf_stream.indexstr);
	}
		break;

	default:
		break;
	}

	va_end(va);
}
