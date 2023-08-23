# compile tool
CC = gcc

# binary name
APP = dirtraval

ifeq ($(DAPP_ROOT_PATH),)
$(error "Please define DAPP_ROOT_PATH environment variable")
endif

# all directory are defined in here
TOOLS_DIR      = $(DAPP_ROOT_PATH)/tools
BUILD_DIR      = $(DAPP_ROOT_PATH)/build
COMMON_DIR     = $(DAPP_ROOT_PATH)/source/common
COMPONENTS_DIR = $(DAPP_ROOT_PATH)/source/components
LIB_DIR        = $(DAPP_ROOT_PATH)/lib

# SRCS
SRCS  = $(TOOLS_DIR)/dir/dir_traval.c
SRCS += $(COMPONENTS_DIR)/dapp_dir_traval.c $(COMPONENTS_DIR)/dapp_queue.c $(COMPONENTS_DIR)/dapp_stack.c

# LIBS
LIBS = 

# CFLAGS
CFLAGS  = -I $(COMMON_DIR)
CFLAGS += -I $(COMPONENTS_DIR)
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
