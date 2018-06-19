##############################################################################
# @Author: ThanNT
# @Date:   05/02/2017
##############################################################################
# Utilitis define
Print = @echo "~"

# Name of build target ex: arm_cortex_m3_base_boot.bin
NAME_MODULE = arm_cortex_m3_base_boot
PROJECT = $(NAME_MODULE)

GCC_PATH	= /home/thannt/workspace/tools/gcc-arm-none-eabi-7-2017-q4-major
OPENOCD_PATH	= /home/thannt/workspace/tools/openocd

#Include sources file
include sources/app/Makefile.mk
include sources/common/Makefile.mk
include sources/driver/Makefile.mk
include sources/sys/Makefile.mk
include sources/platform/stm32l/Makefile.mk

#Boot start address
BOOT_START_ADDR_VAL = 0x08000000

# App start address, that need sync with declare in linker file and interrupt vector table.
APP_START_ADDR_VAL = 0x08003000
APP_ADDR_OPTION = -DAPP_START_ADDR=$(APP_START_ADDR_VAL)

# Define console baudrate, that is used to configure UART.
SYS_CONSOLE_BAUDRATE	= 115200
CONSOLE_BAUDRATE_DEF	= -DSYS_CONSOLE_BAUDRATE=$(SYS_CONSOLE_BAUDRATE)

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

LIBC		= $(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/libc.a
LIBM		= $(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/libm.a
LIBFPU		= $(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/libg.a
LIBGCC		= $(GCC_PATH)/lib/gcc/arm-none-eabi/7.2.1/thumb/v7-m/libgcc.a

LIB_PATH += -L$(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m
LIB_PATH += -L$(GCC_PATH)/lib/gcc/arm-none-eabi/7.2.1/thumb/v7-m

# The command for calling the compiler.
CC		=	$(GCC_PATH)/bin/arm-none-eabi-gcc
CPP		=	$(GCC_PATH)/bin/arm-none-eabi-g++
AR		=	$(GCC_PATH)/bin/arm-none-eabi-ar
LD		=	$(GCC_PATH)/bin/arm-none-eabi-ld
OBJCOPY		=	$(GCC_PATH)/bin/arm-none-eabi-objcopy
ARM_GDB		=	$(GCC_PATH)/bin/arm-none-eabi-gdb
ARM_SIZE	=	$(GCC_PATH)/bin/arm-none-eabi-size

# Set the compiler CPU/FPU options.
CPU = -mcpu=cortex-m3
FPU = -mfloat-abi=soft

# ASM compiler flags
AFLAGS +=	-mthumb	\
		$(CPU) 	\
		$(FPU)	\
		-MD

# Console log compile option
CONSOLE_OPTION +=			\
		-DLOGIN_PRINT_EN	\
		-DAPP_PRINT_EN		\
		-DSYS_PRINT_EN		\
		-DSYS_DBG_EN		\
		-DAPP_DBG_EN		\

# C compiler flags
CFLAGS +=	-mthumb			\
		$(CPU)			\
		$(FPU)			\
		-ffunction-sections	\
		-fdata-sections		\
		-MD			\
		-Wall			\
		-std=c99		\
		-c			\
		$(CONSOLE_BAUDRATE_DEF)	\
		$(CONSOLE_OPTION)	\
		$(OPTIMIZE_OPTION)	\
		$(WARNING_OPTION)	\
		$(RELEASE_OPTION)	\
		$(LOG_AK_KERNEL_ENABLE)	\
		$(APP_ADDR_OPTION)	\
		-DUSE_STDPERIPH_DRIVER	\
		-DSTM32L1XX_MD		\
		-DSTM32L_PLATFORM_BOOT	\

# C++ compiler flags
CPPFLAGS += 	-mthumb			\
		$(CPU)			\
		$(FPU)			\
		-ffunction-sections	\
		-fdata-sections		\
		-fno-rtti		\
		-fno-exceptions		\
		-fno-use-cxa-atexit	\
		-MD			\
		-Wall			\
		-std=c++11		\
		-c			\
		$(CONSOLE_BAUDRATE_DEF)	\
		$(CONSOLE_OPTION)	\
		$(OPTIMIZE_OPTION)	\
		$(WARNING_OPTION)	\
		$(RELEASE_OPTION)	\
		$(LOG_AK_KERNEL_ENABLE)	\
		$(APP_ADDR_OPTION)	\
		-DUSE_STDPERIPH_DRIVER	\
		-DSTM32L1XX_MD		\
		-DSTM32L_PLATFORM_BOOT	\

# linker flags
LDFLAGS	=	-Map=$(OBJECTS_DIR)/$(PROJECT).map	\
		--gc-sections	\
		$(LIB_PATH)	\
		$(LIBC) $(LIBM) $(LIBGCC) $(LIBFPU)

all: create $(TARGET)

create:
	$(Print) CREATE $(OBJECTS_DIR) folder
	@mkdir -p $(OBJECTS_DIR)

$(TARGET): $(OBJECTS) $(LIBC) $(LIBM) $(LIBGCC) $(LIBFPU)
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
	$(Print) CPP $@
	@$(CPP) $(CPPFLAGS) -o $@ $<

$(OBJECTS_DIR)/%.o: %.s
	$(Print) CC $@
	@$(CC) $(AFLAGS) -o $@ -c $<

flash: 	all
	$(Print) BURNING $(TARGET:.axf=.bin) to target
	@st-flash write $(TARGET:.axf=.bin) $(BOOT_START_ADDR_VAL)

debug:	all
	xterm -e openocd -f $(OPENOCD_PATH)/tcl/board/stm32ldiscovery.cfg &
ifeq ($(gdb),ddd)
	$(Print) DBG using DDD
	ddd --debugger $(ARM_GDB) --command=stm32l_init.gdb $(TARGET)
else
	$(Print) DBG GDB
	$(ARM_GDB) --command=stm32l_init.gdb $(TARGET)
endif

com:
	sudo minicom -D /dev/ac_mod_com -b $(SYS_CONSOLE_BAUDRATE)

clean:
	$(Print) CLEAN $(OBJECTS_DIR) folder
	@rm -rf $(OBJECTS_DIR)
