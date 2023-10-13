# compile tool
CC = gcc

# binary name
APP = dapp_deamon

ifeq ($(DAPP_ROOT_PATH),)
$(error "Please execute configure before the first compilation")
endif

# all directory are defined in here
TOOLS_DIR      = $(DAPP_ROOT_PATH)/tools
BUILD_DIR      = $(DAPP_ROOT_PATH)/build
COMMON_DIR     = $(DAPP_ROOT_PATH)/source/common
COMPONENTS_DIR = $(DAPP_ROOT_PATH)/source/components
LIB_DIR        = $(DAPP_ROOT_PATH)/lib

# SRCS
SRCS = $(TOOLS_DIR)/deamon/dapp_deamon.c

# LIBS
LIBS = 

# CFLAGS
CFLAGS  = -I $(COMMON_DIR)
CFLAGS += -I $(COMPONENTS_DIR)
CFLAGS += -g
CFLAGS += -D DAPP_BUILD_VERSION=\"$(BUILD_VERSION)\"

ALL :
	@# check environment variable
	@# create directory
	@if [ ! -d "$(BUILD_DIR)" ]; then mkdir $(BUILD_DIR); fi 
	@if [ ! -d "$(BUILD_DIR)/bin" ]; then mkdir $(BUILD_DIR)/bin; fi

	@echo " make $(APP) tool..."
	@$(CC) $(SRCS) $(LIBS) $(CFLAGS) -o $(BUILD_DIR)/bin/$(APP)
	@echo " make $(APP) tool success!"
	
clean :
	@echo " clean $(APP) tools..."
	@rm $(BUILD_DIR)/bin/$(APP) -rf
	@echo " clean $(APP) success!"

include $(DAPP_ROOT_PATH)/env.mk
