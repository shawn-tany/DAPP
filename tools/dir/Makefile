# compile tool
CC = gcc

# binary name
APP = dirtraval

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
SRCS  = $(TOOLS_DIR)/dir/dir_traval.c
SRCS += $(COMPONENTS_DIR)/dirtraval/dapp_dir_traval.c 
SRCS += $(COMPONENTS_DIR)/structure/dapp_queue.c 
SRCS += $(COMPONENTS_DIR)/structure/dapp_stack.c

# LIBS
LIBS = 

# CFLAGS
CFLAGS  = -I $(COMMON_DIR)
CFLAGS += -I $(COMPONENTS_DIR)/dirtraval
CFLAGS += -I $(COMPONENTS_DIR)/structure

ALL : compile_check compile

clean : clean_build

include $(DAPP_ROOT_PATH)/env.mk
