/**
 ******************************************************************************
 * @author: ThanNT
 * @date:   05/09/2016
 ******************************************************************************
**/
#include <stdint.h>
#include <stdbool.h>

#include "sys_cfg.h"
#include "system.h"
#include "platform.h"

#include "system_stm32l1xx.h"
#include "stm32l1xx.h"
#include "stm32l1xx_conf.h"
#include "core_cm3.h"

#if defined(USING_USB_MOD)
#include "usb_istr.h"
#endif

#include "sys_dbg.h"
#include "sys_ctrl.h"
#include "sys_irq.h"
#include "sys_boot.h"
#include "sys_svc.h"

#include "ak.h"
#include "message.h"
#include "timer.h"

#include "app.h"

#include "sys_cfg.h"

#pragma GCC optimize ("O3")

/*****************************************************************************/
/* linker variable                                                           */
/*****************************************************************************/
extern uint32_t _ldata;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss;
extern uint32_t _ebss;
extern uint32_t _estack;

extern void (*__preinit_array_start[])();
extern void (*__preinit_array_end[])();
extern void (*__init_array_start[])();
extern void (*__init_array_end[])();
extern void _init();

/*****************************************************************************/
/* static function prototype                                                 */
/*****************************************************************************/
/*****************************/
/* system interrupt function */
/*****************************/
void default_handler();
void reset_handler();

/*****************************/
/* user interrupt function   */
/*****************************/
void exti_line1_irq();
void exti_line15_irq();
void timer6_irq();
void timer4_irq();
void timer7_irq();
void usb_lp_irq();

/* cortex-M processor fault exceptions */
void nmi_handler()          __attribute__ ((weak));
void hard_fault_handler()   __attribute__ ((weak));
void mem_manage_handler()   __attribute__ ((weak));
void bus_fault_handler()    __attribute__ ((weak));
void usage_fault_handler()  __attribute__ ((weak));

/* cortex-M processor non-fault exceptions */
void dg_monitor_handler()   __attribute__ ((weak, alias("default_handler")));
void pendsv_handler();
void svc_handler();
void systick_handler();

/* external interrupts */
void shell_handler();
void svc_exe(uint32_t* svc_args);

/*****************************************************************************/
/* system variable                                                           */
/*****************************************************************************/
system_info_t system_info;

/*****************************************************************************/
/* interrupt vector table                                                    */
/*****************************************************************************/
__attribute__((section(".isr_vector")))
void (* const isr_vector[])() = {
		((void (*)())(uint32_t)&_estack),		//	The initial stack pointer
		reset_handler,							//	The reset handler
		nmi_handler,							//	The NMI handler
		hard_fault_handler,						//	The hard fault handler
		mem_manage_handler,						//	The MPU fault handler
		bus_fault_handler,						//	The bus fault handler
		usage_fault_handler,					//	The usage fault handler
		0,										//	Reserved
		0,										//	Reserved
		0,										//	Reserved
		0,										//	Reserved
		svc_handler,							//	SVCall handler
		dg_monitor_handler,						//	Debug monitor handler
		0,										//	Reserved
		pendsv_handler,							//	The PendSV handler
		systick_handler,						//	The SysTick handler

		//External Interrupts
		default_handler,						//	Window Watchdog
		default_handler,						//	PVD through EXTI Line detect
		default_handler,						//	Tamper and Time Stamp
		default_handler,						//	RTC Wakeup
		default_handler,						//	FLASH
		default_handler,						//	RCC
		default_handler,						//	EXTI Line 0
		exti_line1_irq,							//	EXTI Line 1
		default_handler,						//	EXTI Line 2
		default_handler,						//	EXTI Line 3
		default_handler,						//	EXTI Line 4
		default_handler,						//	DMA1 Channel 1
		default_handler,						//	DMA1 Channel 2
		default_handler,						//	DMA1 Channel 3
		default_handler,						//	DMA1 Channel 4
		default_handler,						//	DMA1 Channel 5
		default_handler,						//	DMA1 Channel 6
		default_handler,						//	DMA1 Channel 7
		default_handler,						//	ADC1
		default_handler,						//	USB High Priority
		usb_lp_irq,								//	USB Low  Priority
		default_handler,						//	DAC
		default_handler,						//	COMP through EXTI Line
		default_handler,						//	EXTI Line 9..5
		default_handler,						//	LCD
		default_handler,						//	TIM9
		default_handler,						//	TIM10
		default_handler,						//	TIM11
		default_handler,						//	TIM2
		default_handler,						//	TIM3
		default_handler,						//	TIM4
		default_handler,						//	I2C1 Event
		default_handler,						//	I2C1 Error
		default_handler,						//	I2C2 Event
		default_handler,						//	I2C2 Error
		default_handler,						//	SPI1
		default_handler,						//	SPI2
		shell_handler,							//	USART1
		default_handler,						//	USART2
		default_handler,						//	USART3
		default_handler,						//	EXTI Line 15..10
		default_handler,						//	RTC Alarm through EXTI Line
		sys_irq_usb_recv,						//	USB FS Wakeup from suspend
		default_handler,						//	TIM6
		timer7_irq,								//	TIM7
		};

void __attribute__((naked))
sys_ctrl_delay(volatile uint32_t count)
{
	(void)count;
	__asm("    subs    r0, #1\n"
	"    bne     sys_ctrl_delay\n"
	"    bx      lr");
}

static uint32_t millis_current  = 0;

uint32_t sys_ctrl_millis() {
	volatile uint32_t ret;
	ENTRY_CRITICAL();
	ret = millis_current;
	EXIT_CRITICAL();
	return ret;
}

void _init() {
	/* dummy */
}

/*****************************************************************************/
/* static function defination                                                */
/*****************************************************************************/
void default_handler() {
	FATAL("SY", 0xEE);
}

void reset_handler() {
	/* MUST BE disable interrupt */
	__disable_irq();

	volatile uint32_t *pSrc	= &_ldata;
	volatile uint32_t *pDest	= &_data;
	volatile unsigned i, cnt;

	/* init system */
	SystemInit();

	/* copy init data from FLASH to SRAM */
	while(pDest < &_edata) {
		*pDest++ = *pSrc++;
	}

	/* zero bss */
	for (pDest = &_bss; pDest < &_ebss; pDest++) {
		*pDest = 0UL;
	}

	ENTRY_CRITICAL();

	sys_cfg_clock(); /* init system clock */
	sys_cfg_svc(); /* setting svc exception priority */
	sys_cfg_pendsv(); /* setting psv exception priority */
	sys_cfg_tick(); /* system tick 1ms */
	sys_cfg_console(); /* system console */

	/* invoke all static constructors */
	cnt = __preinit_array_end - __preinit_array_start;
	for (i = 0; i < cnt; i++)
		__preinit_array_start[i]();

	_init();

	cnt = __init_array_end - __init_array_start;
	for (i = 0; i < cnt; i++)
		__init_array_start[i]();

	/* wait configuration stable */
	sys_ctrl_delay(100);  /* wait 300 cycles clock */

	/* update system information */
	sys_cfg_update_info();

	/* entry app function */
	main_app();
}

/***************************************/
/* cortex-M processor fault exceptions */
/***************************************/
void nmi_handler() {
	FATAL("SY", 0x01);
}

void hard_fault_handler() {
	FATAL("SY", 0x02);
}

void mem_manage_handler() {
	FATAL("SY", 0x03);
}

void bus_fault_handler() {
	FATAL("SY", 0x04);
}

void usage_fault_handler() {
	FATAL("SY", 0x05);
}

/*******************************************/
/* cortex-M processor non-fault exceptions */
/*******************************************/
static uint32_t div_counter = 0;

void systick_handler() {
	task_entry_interrupt();

	/* increasing millis counter */
	millis_current++;

	timer_tick(1);

	if (div_counter == 10) {
		div_counter = 0;
	}

	switch (div_counter) {
	case 0:
		sys_irq_timer_10ms();
		break;

	default:
		break;
	}

	div_counter++;

	task_exit_interrupt();
}

void svc_exe(uint32_t* svc_args) {
	volatile uint8_t svc_number;

	sys_dbg_func_stack_dump(svc_args);
	sys_dbg_cpu_dump();

	svc_number = ((uint8_t*)svc_args[6])[-2];

	switch (svc_number) {
	/* start application */
	case 0x01: {

	}
		break;

		/* switch to unprivileged state*/
	case 0x02: {

	}
		break;

	default:
		break;
	}
}

void __attribute__ ((naked)) svc_handler() {
	__asm volatile (
				"tst lr, #4\n"
				"ite eq\n"
				"mrseq r0, msp\n"
				"mrsne r0, psp\n"
				"b svc_exe\n"
				);
}

volatile uint32_t stack_restart_app[8];
volatile uint32_t *sp_restart_app = &stack_restart_app[8];

void __attribute__ ((naked)) pendsv_handler() {
	__disable_irq();

	/* fabricate Cortex-M ISR stack frame for restart application */
	*(--sp_restart_app) = (1U << 24);  /* xPSR */
	*(--sp_restart_app) = (uint32_t)&sys_cfg_restore_app; /* PC */
	*(--sp_restart_app) = 0x00000000U; /* LR  */
	*(--sp_restart_app) = 0x00000000U; /* R12 */
	*(--sp_restart_app) = 0x00000000U; /* R3  */
	*(--sp_restart_app) = 0x00000000U; /* R2  */
	*(--sp_restart_app) = 0x00000000U; /* R1  */
	*(--sp_restart_app) = 0x00000000U; /* R0  */

	__asm volatile ("MSR msp, %0\n" : : "r" (sp_restart_app) : "sp");
	__asm volatile ("bx lr");
}

/************************/
/* external interrupts  */
/************************/
void shell_handler() {
	task_entry_interrupt();

	if (USART_GetITStatus(USARTx, USART_IT_RXNE) == SET) {
		/* DO NOT clear pending interrupt right here ! */
		sys_irq_shell();
	}

	if (USART_GetITStatus(USARTx, USART_IT_TXE) == SET) {
		USART_ClearITPendingBit(USARTx, USART_IT_TXE);
		if (ring_buffer_char_is_empty(&ring_buffer_char_shell_send)) {
			ring_buffer_char_shell_sending_flag = 0;
			USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
		}
		else {
			ring_buffer_char_shell_sending_flag = 1;
			USART_SendData(USARTx, ring_buffer_char_get(&ring_buffer_char_shell_send));
		}
	}

	task_exit_interrupt();
}

void exti_line1_irq() {
	task_entry_interrupt();

	if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
		sys_irq_nrf24l01();
		EXTI_ClearITPendingBit(EXTI_Line1);
	}

	task_exit_interrupt();
}

void exti_line15_irq() {
	task_entry_interrupt();

	if (EXTI_GetITStatus(EXTI_Line15) != RESET) {
		sys_irq_ir_io_rev();
		EXTI_ClearITPendingBit(EXTI_Line15);
	}

	task_exit_interrupt();
}

void timer6_irq() {
	task_entry_interrupt();

	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) {
		sys_irq_timer_50us();
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}

	task_exit_interrupt();
}

void timer7_irq() {
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) {
		sys_ctrl_soft_watchdog_increase_counter();
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}
}

void timer4_irq() {
	task_entry_interrupt();

	if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET) {
		sys_irq_timer_hs1101();
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
	}

	task_exit_interrupt();
}

void usb_lp_irq() {
	task_entry_interrupt();
#if defined(USING_USB_MOD)
	USB_Istr();
#endif
	task_exit_interrupt();
}
