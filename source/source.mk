# compile tool
CC = gcc

# binary name
APP = dapp

# all directory are defined in here
ROOT_DIR ?= $(PWD)/..
SOURCE_DIR = $(ROOT_DIR)/source
BUILD_DIR = $(ROOT_DIR)/build

# all source are stored in SRCS
SRCS = $(SOURCE_DIR)/main.c

# all library are stored in SRCS

CFLAGS += -g
CFLAGS += -D DAPP_DEBUG
CFLAGS += -O0

ALL :
	$(CC) $(CFLAGS) $(SRCS) -o $(BUILD_DIR)/$(APP) $(LIBS)

include $(SOURCE_DIR)/common/common.mk
include $(SOURCE_DIR)/config/config.mk
include $(SOURCE_DIR)/dpdk-api/dpdk-api.mk
include $(ROOT_DIR)/lib/lib.mk
