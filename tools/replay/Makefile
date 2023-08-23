# compile tool
CC = gcc

# binary name
APP = pcap_replay

ifeq ($(DAPP_ROOT_PATH),)
$(error "Please define DAPP_ROOT_PATH environment variable")
endif

# extern environment
TOOLS_DIR  = $(DAPP_ROOT_PATH)/tools
BUILD_DIR  = $(DAPP_ROOT_PATH)/build
COMMON_DIR = $(DAPP_ROOT_PATH)/source/common
LIB_DIR    = $(DAPP_ROOT_PATH)/lib

# SRCS
SRCS  = $(TOOLS_DIR)/replay/pcap_replay.c
SRCS += $(COMMON_DIR)/dapp_dir_traval.c $(COMMON_DIR)/dapp_queue.c $(COMMON_DIR)/dapp_stack.c 

# LIBS
LIBS  = -L $(LIB_DIR)/dpdk/lib -ldpdk -Wl,-rpath=$(LIB_DIR)/dpdk/lib
LIBS += -L $(LIB_DIR)/pcap/lib -lpcap -Wl,-rpath=$(LIB_DIR)/pcap/lib
LIBS += -lpthread -lnuma -ldl -march=native

# CFLAGS
CFLAGS  = -I $(LIB_DIR)/dpdk/include
CFLAGS += -I $(LIB_DIR)/pcap/include
CFLAGS += -I $(COMMON_DIR)
CFLAGS += -g

ALL :
	@# check environment variable
	@# create directory
	@if [ ! -d "$(BUILD_DIR)" ]; then mkdir $(BUILD_DIR); fi 
	@if [ ! -d "$(BUILD_DIR)/bin" ]; then mkdir $(BUILD_DIR)/bin; fi

	@echo " make $(APP) tool..."
	@$(CC) $(SRCS) $(LIBS) $(CFLAGS) -o $(BUILD_DIR)/bin/$(APP)
	@echo " make $(APP) tool success!"

clean :
	@echo " clean $(APP)..."
	@rm $(BUILD_DIR)/bin/$(APP) -rf
	@echo " clean $(APP) success!"
