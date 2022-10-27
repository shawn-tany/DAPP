APP = dapp

COMMON_DIR = common
CONFIG_DIR = config
LIB_DIR = ../lib
DPDK_API_DIR = dpdk-api
BUILD_DIR = ../build

SRCS = main.c 
SRCS += $(CONFIG_DIR)/config.c
SRCS += $(DPDK_API_DIR)/dpdk.c

CFLAGS = -I $(COMMON_DIR)
CFLAGS += -I $(CONFIG_DIR)
CFLAGS += -I $(LIB_DIR)
CFLAGS += -I $(DPDK_API_DIR)
CFLAGS += -g
CFLAGS += -DDAPP_DEBUG

LIBS = $(LIB_DIR)/libjansson.a

ifeq ($(RTE_SDK),)
$(error "Please define RTE_SDK environment variable")
endif

include $(RTE_SDK)/mk/rte.vars.mk

CFLAGS += -O3
CFLAGS += $(WERROR_FLAGS)

include $(RTE_SDK)/mk/rte.extapp.mk

ALL :
	gcc $(SRCS) $(CFLAGS) $(LIBS) -o $(BUILD_DIR)/$(APP)

clean :
	rm $(APP)