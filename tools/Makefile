# compile tool
CC = gcc

# binary name
APP_PCAP_REPLAY = pcap_replay
APP_HS_DB_COMPILE = hs_db_compile
APP_DIR_TRAVAL = dir_traval

ifeq ($(DAPP_ROOT_PATH),)
$(error "Please define DAPP_ROOT_PATH environment variable")
endif

# all directory are defined in here
TOOLS_DIR = $(DAPP_ROOT_PATH)/tools
BUILD_DIR = $(DAPP_ROOT_PATH)/build
COMMON_DIR = $(DAPP_ROOT_PATH)/source/common

TOOLS_LIB_DIR = $(DAPP_ROOT_PATH)/lib

# pcap replay
# pcap replay source are stored in SRCS
PCAP_REPLAY_SRCS = $(TOOLS_DIR)/pcap_replay.c
PCAP_REPLAY_SRCS += $(COMMON_DIR)/dapp_dir_traval.c $(COMMON_DIR)/dapp_queue.c $(COMMON_DIR)/dapp_stack.c 

# pcap replay library are stored in LIBS
PCAP_REPLAY_LIBS = -L $(TOOLS_LIB_DIR)/dpdk/lib -ldpdk -Wl,-rpath=$(TOOLS_LIB_DIR)/dpdk/lib
PCAP_REPLAY_LIBS += -lpthread -lnuma -ldl -march=native -lpcap

# pcap replay flags are stored in CFLAGS
PCAP_REPLAY_CFLAGS = -I $(TOOLS_LIB_DIR)/dpdk/include
PCAP_REPLAY_CFLAGS += -I $(COMMON_DIR)
PCAP_REPLAY_CFLAGS += -g


# hyperscan database compile
# hyperscan database compile source are stored in SRCS
HS_DB_COMPILE_SRCS = $(TOOLS_DIR)/hs_db_compile.c

# hyperscan database compile library are stored in LIBS
HS_DB_COMPILE_LIBS = -L $(TOOLS_LIB_DIR)/hyperscan/lib -lhs -Wl,-rpath=$(TOOLS_LIB_DIR)/hyperscan/lib

# hyperscan database compile flags are stored in CFLAGS
HS_DB_COMPILE_CFLAGS = -I $(TOOLS_LIB_DIR)/hyperscan/include
HS_DB_COMPILE_CFLAGS += -g

# directory traval
# directory traval source are stored in SRCS
DIR_TRAVAL_SRCS = $(TOOLS_DIR)/dir_traval.c
DIR_TRAVAL_SRCS += $(COMMON_DIR)/dapp_dir_traval.c $(COMMON_DIR)/dapp_queue.c $(COMMON_DIR)/dapp_stack.c

# directory traval library are stored in LIBS
DIR_TRAVAL_LIBS = 

# directory traval flags are stored in CFLAGS
DIR_TRAVAL_CFLAGS = -I $(COMMON_DIR)
DIR_TRAVAL_CFLAGS += -g

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

	@echo " make directory traval tool..."
	@$(CC) $(DIR_TRAVAL_SRCS) $(DIR_TRAVAL_LIBS) $(DIR_TRAVAL_CFLAGS) -o $(BUILD_DIR)/bin/$(APP_DIR_TRAVAL)
	@echo " make directory traval tool success!"
	
	@# compile
	@echo " make success!"

clean :
	@echo " clean TOOLS..."
	@rm $(BUILD_DIR)/bin/$(APP_PCAP_REPLAY) -rf
	@rm $(BUILD_DIR)/bin/$(APP_HS_DB_COMPILE) -rf
	@echo " clean success!"
