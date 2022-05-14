## Name
##   Makefile
##
## Purpose
##   Definitions and Rules for Thermometer project
##
## Revision
##    07-Dec-2020 (SSB) [] Initial
##    18-Dec-2021 (SSB) [] Add support for updated 1-Wire library makefile

# Get project name from root directory name
PROJECT_NAME := $(notdir $(CURDIR))

BASE_DIR := ../base
LIBS_DIR := ../libs

STM_CUBE_LIB_VER  := FW_F1_V1.8.0
STM_CUBE_LIB_ROOT := $(LIBS_DIR)/STM32Cube_$(STM_CUBE_LIB_VER)/Drivers

ONE_WIRE_LIB_ROOT := $(LIBS_DIR)/one_wire
UART_LIB_ROOT     := $(LIBS_DIR)/uart

include $(BASE_DIR)/oshelpers.mk

ifeq ($(HOST_OS),)
    $(error "Host OS not defined!")
endif

ifeq ($(BUILD_TYPE),)
    $(error "Build type not defined!")
endif

ifeq ($(MCU),)
    $(error "MCU not defined!")
else ifeq ($(MCU),STM32F100xB)
    CPU_FAMILY := M3
    # Linker script without extension
    LDSCRIPT   := source/stm32f100xb_flash
else
    $(error "MCU not supported!")
endif

# Output directory definitons
BUILD_DIR := build_$(HOST_OS)_$(BUILD_TYPE)
OBJ_DIR   := $(BUILD_DIR)/app/obj
BIN_DIR   := $(BUILD_DIR)/app/bin
DEP_DIR   := $(BUILD_DIR)/app/dep

ifeq ($(TOOLCHAIN),)
    $(error "Toolchain not defined!")
else
    include $(BASE_DIR)/toolchain_$(TOOLCHAIN).mk
endif

# Add libraries to the build
include $(ONE_WIRE_LIB_ROOT)/one_wire.mk
include $(UART_LIB_ROOT)/uart.mk

# Source file directories
SRC_ROOT_DIR := source

# List of object files to be compiled
APP_OBJ_LIST := cli.o \
                cli_ds18b20.o \
                cli_sys.o \
                gpio.o \
                interrupt.o \
                led.o \
                main.o \
                system_init.o \
                time.o \
                tlc59282.o

SYS_OBJ_LIST := system_stm32f1xx.o

ifeq ($(MCU),STM32F100xB)
    SYS_OBJ_LIST += startup_stm32f100xb_$(TOOLCHAIN).o
endif

ifeq ($(TOOLCHAIN),gcc)
    SYS_OBJ_LIST += syscalls.o
endif

STM_HAL_OBJ_LIST := stm32f1xx_hal.o \
                    stm32f1xx_hal_cortex.o \
                    stm32f1xx_hal_gpio.o \
                    stm32f1xx_hal_pwr.o \
                    stm32f1xx_hal_rcc.o \
                    stm32f1xx_hal_spi.o \
                    stm32f1xx_hal_tim.o \
                    stm32f1xx_hal_tim_ex.o \
                    stm32f1xx_hal_uart.o

vpath %.c $(SRC_ROOT_DIR)/application \
          $(SRC_ROOT_DIR)/application/src \
          $(STM_CUBE_LIB_ROOT)/STM32F1xx_HAL_Driver/Src \

vpath %.s $(SRC_ROOT_DIR)/application

# Header file directories
CC_INC_DIR_GCC := $(STM_CUBE_LIB_ROOT)/CMSIS/Include
CC_INC_DIR_IAR := $(TOOLCHAIN_ROOT)/CMSIS/Core/Include

CC_INC_DIR  += $(LIBS_DIR) \
               $(SRC_ROOT_DIR)/application/include \
               $(CC_INC_DIR_$(call toupper,$(TOOLCHAIN))) \
               $(STM_CUBE_LIB_ROOT)/CMSIS/Device/ST/STM32F1xx/Include \
               $(STM_CUBE_LIB_ROOT)/STM32F1xx_HAL_Driver/Inc \
               $(STM_CUBE_LIB_ROOT)/STM32F1xx_HAL_Driver/Inc/Legacy \

# Application specific defines to be forwarded to the command line build
APP_DEFS := -DUSE_HAL_DRIVER \
            -D$(MCU)

OBJ_LIST := $(addprefix $(OBJ_DIR)/,$(APP_OBJ_LIST))
OBJ_LIST += $(addprefix $(OBJ_DIR)/,$(SYS_OBJ_LIST))

STM_HAL_LIB_OBJ_DIR  := $(BUILD_DIR)/stm/hal/obj
STM_HAL_LIB_OUT_DIR  := $(BUILD_DIR)/stm/hal/lib
STM_HAL_LIB_OBJ_LIST := $(addprefix $(STM_HAL_LIB_OBJ_DIR)/,$(STM_HAL_OBJ_LIST))
STM_HAL_LIB          := stm32_hal.a

# Library directories
CC_LIBS_DIR += $(STM_HAL_LIB_OUT_DIR)

CC_INC_FLAG   := -I
CC_INC_PARAMS += $(addprefix $(CC_INC_FLAG),$(CC_INC_DIR))

CC_LINK_INC_FLAG   := -L
CC_LINK_INC_PARAMS := $(addprefix $(CC_LINK_INC_FLAG),$(CC_LIBS_DIR))

CC_LINK_FLAG := -l
CC_LINK_LIBS += $(addprefix $(CC_LINK_FLAG),$(basename $(STM_HAL_LIB)))

# Add libraries to the executable
CC_LIBS += $(STM_HAL_LIB)

include $(wildcard $(DEP_DIR)/*.d)

#
# Build rules
#

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR) $(DEP_DIR)
	@echo "Compiling $@"
	$(CC) $(CFLAGS) $(DEP_FLAGS) $(DEP_DIR)/$*.d $(APP_DEFS) $(CC_INC_PARAMS) -o $@ -c $<

$(OBJ_DIR)/%.o: %.s | $(OBJ_DIR)
	@echo "Assembling $@"
	$(AS) $(ASFLAGS) -o $@ -c $<

$(TARGET_ELF): $(CC_LIBS) $(OBJ_LIST) | $(BIN_DIR)
	@echo "Linking $@"
	$(LD) -o $@ $(OBJ_LIST) $(LDFLAGS) $(CC_LINK_INC_PARAMS) $(CC_LINK_LIBS)

$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCPY) $(OBJCPY_BIN_FLAGS) $< $@

$(TARGET_HEX): $(TARGET_ELF)
	$(OBJCPY) $(OBJCPY_HEX_FLAGS) $< $@

size: $(TARGET_ELF)
	$(SIZE) $(TARGET_ELF)

$(STM_HAL_LIB_OBJ_DIR)/%.o: %.c | $(STM_HAL_LIB_OBJ_DIR)
	@echo "Compiling $@"
	$(CC) $(CFLAGS) $(APP_DEFS) $(CC_INC_PARAMS) -o $@ -c $<

$(STM_HAL_LIB): $(STM_HAL_LIB_OBJ_LIST) | $(STM_HAL_LIB_OUT_DIR)
	@echo "Building library $@"
	$(AR) $(ARFLAGS) $(STM_HAL_LIB_OUT_DIR)/lib$@ $^

$(OBJ_DIR):
	$(MD) $@

$(BIN_DIR):
	$(MD) $@

$(DEP_DIR):
	$(MD) $@

$(STM_HAL_LIB_OBJ_DIR):
	$(MD) $@

$(STM_HAL_LIB_OUT_DIR):
	$(MD) $@

flash:
	st-flash erase
	st-flash write $(TARGET_BIN) 0x8000000

ifeq ($(TOOLCHAIN),gcc)
    EXT_TARGET := size
endif

#
# Recipes
#

all: $(CC_LIBS) $(TARGET_ELF) $(TARGET_BIN) $(TARGET_HEX) $(EXT_TARGET)

clean:
	$(RM) $(OBJ_DIR)
	$(RM) $(BIN_DIR)
	$(RM) $(DEP_DIR)

clean_all:
	$(RM) $(BUILD_DIR)

.PHONY: all clean clean_all
