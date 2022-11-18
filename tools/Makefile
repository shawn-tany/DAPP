# compile tool
CC = gcc

# binary name
APP_PCAP_REPLAY = pcap_replay

ifeq ($(DAPP_ROOT_PATH),)
$(error "Please define DAPP_ROOT_PATH environment variable")
endif

# all directory are defined in here
TOOLS_DIR = $(DAPP_ROOT_PATH)/tools
BUILD_DIR = $(DAPP_ROOT_PATH)/build

TOOLS_LIB_DIR = $(DAPP_ROOT_PATH)/lib

# all source are stored in SRCS
TOOLS_SRCS = $(TOOLS_DIR)/pcap_replay.c

# all library are stored in LIBS
TOOLS_LIBS = -L $(TOOLS_LIB_DIR)/dpdk/lib -ldpdk -Wl,-rpath=$(TOOLS_LIB_DIR)/dpdk/lib
TOOLS_LIBS += -lpthread -lnuma -ldl -march=native -lpcap

TOOLS_CFLAGS = -I $(TOOLS_LIB_DIR)/dpdk/include
TOOLS_CFLAGS += -g

ALL :
	@# check environment variable
	@echo " make TOOLS..."
	@# create directory
	@if [ ! -d "$(BUILD_DIR)" ]; then mkdir $(BUILD_DIR); fi 
	@if [ ! -d "$(BUILD_DIR)/bin" ]; then mkdir $(BUILD_DIR)/bin; fi

	@$(CC) $(TOOLS_SRCS) $(TOOLS_LIBS) $(TOOLS_CFLAGS) -o $(BUILD_DIR)/bin/$(APP_PCAP_REPLAY)
	
	@# compile
	@echo " make finish"

clean :
	@echo " clean TOOLS..."
	@rm $(BUILD_DIR)/$(APP_PCAP_REPLAY) -rf
	@echo " clean finish"
