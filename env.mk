BUILD_VERSION = $(shell $(DAPP_ROOT_PATH)/version.sh)

CFLAGS += -Wall -Werror
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -D DAPP_DEBUG
CFLAGS += -D DAPP_CONFIG_PATH=\"$(DAPP_INSTALL_PATH)/config\"
CFLAGS += -D DAPP_CACHE_PATH=\"$(DAPP_INSTALL_PATH)/cache\"
CFLAGS += -D DAPP_LOG_PATH=\"$(DAPP_INSTALL_PATH)/log\"
CFLAGS += -D DAPP_BUILD_VERSION=\"$(BUILD_VERSION)\"

ifneq ($(version),)
    BUILD_VERSION=$(version)
endif

quit = no
ifeq ($(quit),yes)
    quit = yes
endif

compile_check :
	@# create directory
	@if [ ! -d "$(BUILD_DIR)" ]; then mkdir $(BUILD_DIR); fi 
	@if [ ! -d "$(BUILD_DIR)/bin" ]; then mkdir $(BUILD_DIR)/bin; fi
	@if [ ! -d "$(BUILD_DIR)/obj" ]; then mkdir $(BUILD_DIR)/obj; fi
	@if [ ! -d "$(BUILD_DIR)/obj/$(APP)" ]; then mkdir $(BUILD_DIR)/obj/$(APP); fi

install_check :
	@if [ -d "$(DAPP_INSTALL_PATH_LAST)" ]; then rm $(DAPP_INSTALL_PATH_LAST) -rf; fi
	@if [ ! -d "$(DAPP_INSTALL_PATH)" ]; then mkdir $(DAPP_INSTALL_PATH); fi
	@if [ ! -d "$(DAPP_INSTALL_PATH)/cache" ]; then mkdir $(DAPP_INSTALL_PATH)/cache; fi
	@if [ ! -d "$(DAPP_INSTALL_PATH)/log" ]; then mkdir $(DAPP_INSTALL_PATH)/log; fi
	@if [ ! -d "$(PACKAGE_DIR)" ]; then mkdir $(PACKAGE_DIR); fi
	@if [ ! -d "$(PACKAGE_DIR)/bin" ]; then mkdir $(PACKAGE_DIR)/bin; fi
	@if [ ! -d "$(PACKAGE_DIR)/tools" ]; then mkdir $(PACKAGE_DIR)/tools; fi

ifneq ($(quit),yes)
compile :
	@echo "  ## build $(APP)"
	@for src in $(SRCS);            \
	do                              \
	    ($(CC) $(CFLAGS) $(LIBS) $$src -c) && (echo "  [SUCCESS] $$src") || ((echo "  [FAILED ] $$src" && (rm *.o -rf)) && (exit 1)); \
	done;
	@mv *.o $(BUILD_DIR)/obj/$(APP)
	@$(CC) $(BUILD_DIR)/obj/$(APP)/*.o -o $(BUILD_DIR)/bin/$(APP)  $(CFLAGS) $(LIBS)
	@echo "  ## build $(APP) success!"
else
compile :
	@echo "  ## build $(APP)..."
	@$(CC) $(SRCS) -o $(BUILD_DIR)/bin/$(APP)  $(CFLAGS) $(LIBS)
	@echo "  ## build $(APP) success!"
endif

clean_build :
	@echo "  ## clean $(APP)..."
	@rm $(BUILD_DIR)/bin/$(APP) -rf
	@rm $(BUILD_DIR)/obj/$(APP) -rf
	@echo "  ## clean $(APP) success!"

show_build_version :
	@echo "  ## build version $(BUILD_VERSION)"

