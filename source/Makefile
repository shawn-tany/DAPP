APP = dapp

COMMON_DIR = common
CONFIG_DIR = config
LIB_DIR = ../lib
DPDK_API_DIR = dpdk-api
BUILD_DIR = ../build

SRCS = main.c 
SRCS += $(CONFIG_DIR)/config.c

CFLAGS = -I $(COMMON_DIR)
CFLAGS += -I $(CONFIG_DIR)
CFLAGS += -I $(LIB_DIR)
CFLAGS += -I $(DPDK_API_DIR)
CFLAGS += -g
CFLAGS += -DDAPP_DEBUG

LIBS = $(LIB_DIR)/libjansson.a

ALL :
	gcc $(SRCS) $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/$(APP)

clean :
	rm $(APP)