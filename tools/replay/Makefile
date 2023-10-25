# compile tool
CC = gcc

# binary name
APP = pcap_replay

ifeq ($(DAPP_ROOT_PATH),)
$(error "Please execute configure before the first compilation")
endif

# extern environment
TOOLS_DIR      = $(DAPP_ROOT_PATH)/tools
BUILD_DIR      = $(DAPP_ROOT_PATH)/build
COMMON_DIR     = $(DAPP_ROOT_PATH)/source/common
COMPONENTS_DIR = $(DAPP_ROOT_PATH)/source/components
LIB_DIR        = $(DAPP_ROOT_PATH)/lib

# SRCS
SRCS  = $(TOOLS_DIR)/replay/pcap_replay.c
SRCS += $(COMPONENTS_DIR)/dapp_dir_traval.c 
SRCS += $(COMPONENTS_DIR)/dapp_queue.c 
SRCS += $(COMPONENTS_DIR)/dapp_stack.c 

# LIBS
LIBS  = -L $(LIB_DIR)/dpdk/lib -ldpdk -Wl,-rpath=$(LIB_DIR)/dpdk/lib
LIBS += -L $(LIB_DIR)/pcap/lib -lpcap -Wl,-rpath=$(LIB_DIR)/pcap/lib
LIBS += -lpthread -lnuma -ldl -march=native

# CFLAGS
CFLAGS  = -I $(LIB_DIR)/dpdk/include
CFLAGS += -I $(LIB_DIR)/pcap/include
CFLAGS += -I $(COMMON_DIR)
CFLAGS += -I $(COMPONENTS_DIR)

ALL : compile_check compile

clean : clean_build

include $(DAPP_ROOT_PATH)/env.mk
