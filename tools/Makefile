# compile tool
CC = gcc

# binary name
APP_PCAP_REPLAY = pcap_replay
APP_HS_DB_COMPILE = hs_db_compile

ifeq ($(DAPP_ROOT_PATH),)
$(error "Please define DAPP_ROOT_PATH environment variable")
endif

# all directory are defined in here
TOOLS_DIR = $(DAPP_ROOT_PATH)/tools
BUILD_DIR = $(DAPP_ROOT_PATH)/build

TOOLS_LIB_DIR = $(DAPP_ROOT_PATH)/lib

# all source are stored in SRCS
PCAP_REPLAY_SRCS = $(TOOLS_DIR)/pcap_replay.c

HS_DB_COMPILE_SRCS = $(TOOLS_DIR)/hs_db_compile.c

# all library are stored in LIBS
PCAP_REPLAY_LIBS = -L $(TOOLS_LIB_DIR)/dpdk/lib -ldpdk -Wl,-rpath=$(TOOLS_LIB_DIR)/dpdk/lib
PCAP_REPLAY_LIBS += -lpthread -lnuma -ldl -march=native -lpcap

HS_DB_COMPILE_LIBS = -L $(TOOLS_LIB_DIR)/hyperscan/lib -lhs -Wl,-rpath=$(TOOLS_LIB_DIR)/hyperscan/lib

# all flags are stored in CFLAGS
PCAP_REPLAY_CFLAGS = -I $(TOOLS_LIB_DIR)/dpdk/include
PCAP_REPLAY_CFLAGS += -g

HS_DB_COMPILE_CFLAGS = -I $(TOOLS_LIB_DIR)/hyperscan/include
HS_DB_COMPILE_CFLAGS += -g

ALL :
	@# check environment variable
	@echo " make TOOLS..."
	@# create directory
	@if [ ! -d "$(BUILD_DIR)" ]; then mkdir $(BUILD_DIR); fi 
	@if [ ! -d "$(BUILD_DIR)/bin" ]; then mkdir $(BUILD_DIR)/bin; fi

	@echo " make pcap replay tool..."
	@$(CC) $(PCAP_REPLAY_SRCS) $(PCAP_REPLAY_LIBS) $(PCAP_REPLAY_CFLAGS) -o $(BUILD_DIR)/bin/$(APP_PCAP_REPLAY)
	@echo " make pcap replay tool success!"

	@echo " make hyperscan database compile tool..."
	@$(CC) $(HS_DB_COMPILE_SRCS) $(HS_DB_COMPILE_LIBS) $(HS_DB_COMPILE_CFLAGS) -o $(BUILD_DIR)/bin/$(APP_HS_DB_COMPILE)
	@echo " make hyperscan database compile tool success!"
	
	@# compile
	@echo " make success!"

clean :
	@echo " clean TOOLS..."
	@rm $(BUILD_DIR)/bin/$(APP_PCAP_REPLAY) -rf
	@rm $(BUILD_DIR)/bin/$(APP_HS_DB_COMPILE) -rf
	@echo " clean success!"
