# compile tool
CC = gcc

# binary name
APP = dapp_compile

ifeq ($(DAPP_ROOT_PATH),)
$(error "Please execute configure before the first compilation")
endif

# all directory are defined in here
TOOLS_DIR      = $(DAPP_ROOT_PATH)/tools
BUILD_DIR      = $(DAPP_ROOT_PATH)/build
COMMON_DIR     = $(DAPP_ROOT_PATH)/source/common
COMPONENTS_DIR = $(DAPP_ROOT_PATH)/source/common
LIB_DIR        = $(DAPP_ROOT_PATH)/lib

# SRCS
SRCS = $(TOOLS_DIR)/match/dapp_compile.c

# LIBS
LIBS = -L $(LIB_DIR)/hyperscan/lib -lhs -Wl,-rpath=$(LIB_DIR)/hyperscan/lib

# CFLAGS
CFLAGS  = -I $(LIB_DIR)/hyperscan/include

ALL : compile_check compile

clean : clean_build

include $(DAPP_ROOT_PATH)/env.mk
