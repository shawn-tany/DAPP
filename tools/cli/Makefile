# compile tool
CC = gcc

# binary name
APP = dapp_cli

ifeq ($(DAPP_ROOT_PATH),)
$(error "Please execute configure before the first compilation")
endif

# all directory are defined in here
TOOLS_DIR      = $(DAPP_ROOT_PATH)/tools
BUILD_DIR      = $(DAPP_ROOT_PATH)/build
COMMON_DIR     = $(DAPP_ROOT_PATH)/source/common
COMPONENTS_DIR = $(DAPP_ROOT_PATH)/source/components
LIB_DIR        = $(DAPP_ROOT_PATH)/lib

# SRCS
SRCS := $(TOOLS_DIR)/cli/dapp_cli.c

# LIBS
LIBS = 

# CFLAGS
CFLAGS  = -I $(COMMON_DIR)
CFLAGS += -I $(COMPONENTS_DIR)

ALL : compile_check compile

clean : clean_build

include $(DAPP_ROOT_PATH)/env.mk
