# compile tool
CC = gcc

# binary name
APP = dapp

ifeq ($(DAPP_ROOT_PATH),)
$(error "Please execute configure before the first compilation")
endif

# all directory are defined in here
SOURCE_DIR = $(DAPP_ROOT_PATH)/source
BUILD_DIR  = $(DAPP_ROOT_PATH)/build

# all source are stored in SRCS
SRCS = $(SOURCE_DIR)/main.c

# all library are stored in LIBS

ALL : compile_check show_build_version compile

clean : clean_build

include $(DAPP_ROOT_PATH)/env.mk
include $(DAPP_ROOT_PATH)/lib/lib.mk
include $(SOURCE_DIR)/dpdk/dpdk.mk
include $(SOURCE_DIR)/common/common.mk
include $(SOURCE_DIR)/components/components.mk
include $(SOURCE_DIR)/config/config.mk
include $(SOURCE_DIR)/modules/modules.mk

