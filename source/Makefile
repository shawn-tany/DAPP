

# compile tool
CC = gcc

# binary name
APP = dapp

# all directory are defined in here
COMMON_DIR = common
CONFIG_DIR = config
DPDK_API_DIR = dpdk-api
LIB_JANSSON_DIR = ../lib/jansson
LIB_DPDK_DIR = ../lib/dpdk

BUILD_DIR = ../build

# all source are stored in SRCS
SRCS = main.c 
SRCS += $(COMMON_DIR)/modules.c
SRCS += $(CONFIG_DIR)/config.c
SRCS += $(DPDK_API_DIR)/dpdk.c

# all library are stored in SRCS
LIBS = $(LIB_JANSSON_DIR)/lib/libjansson.a 
LIBS += $(LIB_DPDK_DIR)//lib/libdpdk.a 

CFLAGS += -I $(COMMON_DIR)
CFLAGS += -I $(CONFIG_DIR)
CFLAGS += -I $(DPDK_API_DIR)

CFLAGS += -I $(LIB_JANSSON_DIR)/include
CFLAGS += -I $(LIB_DPDK_DIR)/include

CFLAGS += -g
CFLAGS += -DDAPP_DEBUG
CFLAGS += -O0

CFLAGS += -lpthread -lnuma -ldl

ifeq ($(RTE_SDK),)
$(error "Please define RTE_SDK environment variable")
endif

ALL :
	$(CC) $(CFLAGS) $(SRCS) -o $(BUILD_DIR)/$(APP) $(LIBS)
