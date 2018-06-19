##############################################################################
# @author: ThanNT
# @date:   05/2/2017
##############################################################################
# Utilitis define
Print = @echo "~"

# Name of build target ex: arm_m3_base_app.bin
NAME_MODULE = arm_m3_base_app
PROJECT = $(NAME_MODULE)

GCC_PATH	= /home/thannt/workspace/tools/gcc-arm-none-eabi-7-2017-q4-major
OPENOCD_PATH	= /home/thannt/workspace/tools/openocd

# Include sources file
include sources/ak/Makefile.mk
include sources/app/Makefile.mk
include sources/common/Makefile.mk
include sources/driver/Makefile.mk
include sources/sys/Makefile.mk
include sources/networks/Makefile.mk
include sources/cloud/Makefile.mk
include sources/platform/stm32l/Makefile.mk

# App start address, that need sync with declare in linker file and interrupt vector table.
APP_START_ADDR_VAL = 0x08003000
APP_ADDR_OPTION = -DAPP_START_ADDR=$(APP_START_ADDR_VAL)

# Define console baudrate, that is used to configure UART.
SYS_CONSOLE_BAUDRATE = 115200
CONSOLE_BAUDRATE_DEF = -DSYS_CONSOLE_BAUDRATE=$(SYS_CONSOLE_BAUDRATE)

#|---------------------------------------------------------------------------------------------------|
#| OPTIMIZE LEVELS                                                                                   |
#|------------|----------------------------------|--------------|---------|------------|-------------|
#|   option   | optimization levels              |execution time|code size|memory usage|complile time|
#|------------|----------------------------------|--------------|---------|------------|-------------|
#|   -O0      | compilation time                 |     (+)      |   (+)   |     (-)    |    (-)      |
#| -O1 || -O  | code size && execution time      |     (-)      |   (-)   |     (+)    |    (+)      |
#|   -O2      | more code size && execution time |     (--)     |         |     (+)    |    (++)     |
#|   -O3      | more code size && execution time |     (---)    |         |     (+)    |    (+++)    |
#|   -Os      | code size                        |              |   (--)  |            |    (++)     |
#|  -Ofast    | O3 with none math cals           |     (---)    |         |     (+)    |    (+++)    |
#|------------|----------------------------------|--------------|---------|------------|-------------|
OPTIMIZE_OPTION = -g -O2

# Release build option, that is used in fatal function, restart system when fatal, and debug levels.
RELEASE_OPTION = -DRELEASE

# Enable PIN (using led life) interrupt analyzer function [entry IRQ -> led life ON] and [exit IRQ -> led life OFF].
IRQ_DEBUG_OPTION = -UAK_IO_IRQ_ANALYZER

# Enable ak message log (via UART console).
LOG_AK_KERNEL_ENABLE = -UAK_TASK_LOG_CONSOLE_ENABLE

# Enable to using USB hareware configuration.
USB_OPTION = -UUSING_USB_MOD

# App maximum stack size
APP_STACK_SIZE = 720

# Warning compile option.
WARNING_OPTION = -Werror -W -Wstack-usage=$(APP_STACK_SIZE)

OBJECTS_DIR = build_$(NAME_MODULE)_stm32l
TARGET = $(OBJECTS_DIR)/$(NAME_MODULE).axf

SOURCES		+=
SOURCES_	 = $(shell find $(SOURCES) -type f -printf "%f\n")
OBJECTS		+= $(patsubst %.c, $(OBJECTS_DIR)/%.o, $(SOURCES_))

SOURCES_CPP	+=
SOURCES_CPP_	 = $(shell find $(SOURCES_CPP) -type f -printf "%f\n")
OBJECTS		+= $(patsubst %.cpp, $(OBJECTS_DIR)/%.o, $(SOURCES_CPP_))

SOURCES_ASM	+=
SOURCES_ASM_	 = $(shell find $(SOURCES_ASM) -type f -printf "%f\n")
OBJECTS		+= $(patsubst %.s, $(OBJECTS_DIR)/%.o, $(SOURCES_ASM_))

LIBC		= $(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/libc.a
LIBM		= $(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/libm.a
LIBFPU		= $(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/libg.a
LIBGCC		= $(GCC_PATH)/lib/gcc/arm-none-eabi/7.2.1/thumb/v7-m/libgcc.a
LIBAK		= sources/ak/lib/libak.a

LIB_PATH += -L$(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m
LIB_PATH += -L$(GCC_PATH)/lib/gcc/arm-none-eabi/7.2.1/thumb/v7-m
LIB_PATH += -Lsources/ak/lib

# The command for calling the compiler.
CC		=	$(GCC_PATH)/bin/arm-none-eabi-gcc
CPP		=	$(GCC_PATH)/bin/arm-none-eabi-g++
AR		=	$(GCC_PATH)/bin/arm-none-eabi-ar
AS		=	$(GCC_PATH)/bin/arm-none-eabi-gcc -x assembler-with-cpp
LD		=	$(GCC_PATH)/bin/arm-none-eabi-ld
OBJCOPY		=	$(GCC_PATH)/bin/arm-none-eabi-objcopy
OBJDUMP		=	$(GCC_PATH)/bin/arm-none-eabi-objdump
OBJNM		=	$(GCC_PATH)/bin/arm-none-eabi-nm
ARM_GDB		=	$(GCC_PATH)/bin/arm-none-eabi-gdb
ARM_SIZE	=	$(GCC_PATH)/bin/arm-none-eabi-size

# Set the compiler CPU/FPU options.
CPU = -mcpu=cortex-m3
FPU = -mfloat-abi=soft

# Console log compile option
CONSOLE_OPTION +=			\
		-DLOGIN_PRINT_EN	\
		-DSYS_PRINT_EN		\
		-DSYS_DBG_EN		\
		-UAPP_DBG_EN		\
		-UAPP_PRINT_EN		\
		-UAPP_DBG_SIG_EN	\

# C compiler flags
CFLAGS +=	-mthumb			\
		$(CPU)			\
		$(FPU)			\
		-ffunction-sections	\
		-fdata-sections		\
		-fstack-usage		\
		-MD			\
		-Wall			\
		-std=c99		\
		-c			\
		$(CONSOLE_BAUDRATE_DEF)	\
		$(CONSOLE_OPTION)	\
		$(IRQ_DEBUG_OPTION)	\
		$(WARNING_OPTION)	\
		$(OPTIMIZE_OPTION)	\
		$(RELEASE_OPTION)	\
		$(LOG_AK_KERNEL_ENABLE)	\
		$(APP_ADDR_OPTION)	\
		$(USB_OPTION)		\
		-DUSE_STDPERIPH_DRIVER	\
		-DSTM32L1XX_MD		\
		-DSTM32L_PLATFORM	\


# C++ compiler flags
CPPFLAGS += 	-mthumb			\
		$(CPU)			\
		$(FPU)			\
		-ffunction-sections	\
		-fdata-sections		\
		-fstack-usage		\
		-fno-rtti		\
		-fno-exceptions		\
		-fno-use-cxa-atexit	\
		-MD			\
		-Wall			\
		-std=c++11		\
		-c			\
		$(CONSOLE_BAUDRATE_DEF)	\
		$(CONSOLE_OPTION)	\
		$(IRQ_DEBUG_OPTION)	\
		$(WARNING_OPTION)	\
		$(OPTIMIZE_OPTION)	\
		$(RELEASE_OPTION)	\
		$(LOG_AK_KERNEL_ENABLE)	\
		$(APP_ADDR_OPTION)	\
		$(USB_OPTION)		\
		-DUSE_STDPERIPH_DRIVER	\
		-DSTM32L1XX_MD		\
		-DSTM32L_PLATFORM	\

# linker flags
LDFLAGS	=	-Map=$(OBJECTS_DIR)/$(PROJECT).map	\
		--gc-sections	\
		$(LIB_PATH)	\
		$(LIBC) $(LIBM) $(LIBGCC) $(LIBFPU) $(LIBAK)

all: create $(TARGET)

create:
	$(Print) CREATE $(OBJECTS_DIR) folder
	@mkdir -p $(OBJECTS_DIR)

$(TARGET): $(OBJECTS) $(LIBC) $(LIBM) $(LIBGCC) $(LIBFPU) $(LIBAK)
	$(Print) LD $@
	@$(LD) --entry reset_handler -T $(LDFILE) $(LDFLAGS) -o $(@) $(^)
	$(Print) OBJCOPY $(@:.axf=.bin)
	@$(OBJCOPY) -O binary $(@) $(@:.axf=.bin)
	@$(OBJCOPY) -O binary $(@) $(@:.axf=.out)
	@$(OBJCOPY) -O binary $(@) $(@:.axf=.elf)
	@$(ARM_SIZE) $(TARGET)

$(OBJECTS_DIR)/%.o: %.c
	$(Print) CC $@
	@$(CC) $(CFLAGS) -o $@ $<

$(OBJECTS_DIR)/%.o: %.cpp
	$(Print) CXX $@
	@$(CPP) $(CPPFLAGS) -o $@ $<

$(OBJECTS_DIR)/%.o: %.s
	$(Print) AS $@
	@$(AS) $(CFLAGS) -o $@ $<

asm: $(TARGET)
	$(Print) disassemble-all $(<:.axf=.asm)
	@$(OBJDUMP) -D $(<) >> $(<:.axf=.asm)

sym: $(TARGET)
	$(Print) export object name $(<:.axf=.sym)
	@$(OBJNM) $(<) >> $(<:.axf=.sym)

.PHONY: help
help:
	$(Print) "[make flash dev=ak-flash] burn firmware via ak bootloader"
	$(Print) "[make flash] burn firmware via st-link"
	$(Print) "             NOTE: This option is needed install ak_flash, github: https://github.com/thannt92/ak_flash.git"
	$(Print) "[make debug gdb=ddd] gdb debug using ddd"
	$(Print) "[make debug] gdb debug"
	$(Print) "[make com] open minicom using device /dev/thesis_com_port"
	$(Print) "[make clean] clean build project folder"
	$(Print) "[make asm] generate asm file"
	$(Print) "[make sym] list symbol from objects file"

.PHONY: flash
flash: 	all
	$(Print) BURNING $(TARGET:.axf=.bin) to target
ifeq ($(dev),ak-flash)
	ak_flash /dev/thesis_com_port $(TARGET:.axf=.bin) $(APP_START_ADDR_VAL)
else
	st-flash write $(TARGET:.axf=.bin) $(APP_START_ADDR_VAL)
endif

.PHONY: debug
debug:	all
	xterm -e openocd -f $(OPENOCD_PATH)/tcl/board/stm32ldiscovery.cfg &
ifeq ($(gdb),ddd)
	$(Print) DBG using DDD
	ddd --debugger $(ARM_GDB) --command=stm32l_init.gdb $(TARGET)
else
	$(Print) DBG GDB
	$(ARM_GDB) -w --command=stm32l_init.gdb $(TARGET)
endif

.PHONY: com
com:
	minicom -D /dev/thesis_com_port -b $(SYS_CONSOLE_BAUDRATE)

.PHONY: clean
clean:
	$(Print) CLEAN $(OBJECTS_DIR) folder
	@rm -rf $(OBJECTS_DIR)
