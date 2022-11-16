# compile tool
CC = gcc

# binary name
APP_PCAP_REPLAY = pcap_replay

# all directory are defined in here
ROOT_DIR ?= $(PWD)/..
TOOLS_DIR = $(ROOT_DIR)/tools
TOOLS_BUILD_DIR = $(TOOLS_DIR)/build

TOOLS_LIB_DIR = $(ROOT_DIR)/lib

# all source are stored in SRCS
TOOLS_SRCS = $(TOOLS_DIR)/pcap_replay.c

# all library are stored in LIBS
TOOLS_LIBS = -L $(TOOLS_LIB_DIR)/dpdk/lib -ldpdk -Wl,-rpath=$(TOOLS_LIB_DIR)/dpdk/lib
TOOLS_LIBS += -lpthread -lnuma -ldl -march=native -lpcap

TOOLS_CFLAGS = -I $(TOOLS_LIB_DIR)/dpdk/include
TOOLS_CFLAGS += -g

TOOLS_ALL :
	@echo " make TOOLS..."
	@# create directory
	@if [ ! -d "$(TOOLS_BUILD_DIR)" ]; then mkdir $(TOOLS_BUILD_DIR); fi 
	@if [ ! -d "$(TOOLS_BUILD_DIR)/bin" ]; then mkdir $(TOOLS_BUILD_DIR)/bin; fi

	@$(CC) $(TOOLS_SRCS) $(TOOLS_LIBS) $(TOOLS_CFLAGS) -o $(TOOLS_BUILD_DIR)/bin/$(APP_PCAP_REPLAY)
	
	@# compile
	@echo " make finish"

tools_clean :
	@echo " clean TOOLS..."
	@rm $(TOOLS_BUILD_DIR) -rf
	@echo " clean finish"