# System program installation directory
SYS_SBIN_DIR = /usr/sbin

# DAPP installation directory
SYS_INSTALL_DIR = /etc/DAPP

# DAPP installation package directory
PACKAGE_DIR = $(DAPP_ROOT_PATH)/package

# DAPP installation package
PACKAGE = dapp-install-package.tar.gz

# Common variable
# DAPP tools directory
TOOLS_DIR = $(DAPP_ROOT_PATH)/tools

# DAPP build directory
BUILD_DIR = $(DAPP_ROOT_PATH)/build

# pcap replay tool
APP_PCAP_REPLAY = pcap_replay

# dapp binary
APP = dapp

.PHONY : clean install uninstall

ifeq ($(DAPP_ROOT_PATH),)
$(error "Please define DAPP_ROOT_PATH environment variable")
endif

ALL :
	@make --no-print-directory -f $(DAPP_ROOT_PATH)/source/Makefile
	@make --no-print-directory -f $(DAPP_ROOT_PATH)/tools/Makefile

clean :
	@make --no-print-directory -f source/Makefile clean
	@make --no-print-directory -f tools/Makefile clean

install :
	@echo " install DAPP..."
	@if [ ! -d "$(SYS_INSTALL_DIR)" ]; then mkdir $(SYS_INSTALL_DIR); fi
	@if [ ! -d "$(SYS_INSTALL_DIR)/cache" ]; then mkdir $(SYS_INSTALL_DIR)/cache; fi
	@if [ ! -d "$(PACKAGE_DIR)" ]; then mkdir $(PACKAGE_DIR); fi
	@if [ ! -d "$(PACKAGE_DIR)/bin" ]; then mkdir $(PACKAGE_DIR)/bin; fi
	@if [ ! -d "$(PACKAGE_DIR)/tools" ]; then mkdir $(PACKAGE_DIR)/tools; fi
	@if [ ! -d "$(PACKAGE_DIR)/tools/bin" ]; then mkdir $(PACKAGE_DIR)/tools/bin; fi

	@# update package
	@cp $(BUILD_DIR)/bin/$(APP) $(PACKAGE_DIR)/bin -rf
	@cp $(BUILD_DIR)/bin/$(APP_PCAP_REPLAY) $(PACKAGE_DIR)/bin -rf
	@cp $(TOOLS_DIR)/*.sh $(PACKAGE_DIR)/tools -rf
	@cp $(TOOLS_DIR)/*.py $(PACKAGE_DIR)/tools -rf

	@# Create Package
	@rm $(PACKAGE_DIR)/$(PACKAGE) -rf
	@tar -zcPf $(PACKAGE_DIR)/$(PACKAGE) $(PACKAGE_DIR)/*

	@# Install to system directory
	@cp $(PACKAGE_DIR)/* $(SYS_INSTALL_DIR) -rf
	@cp $(PACKAGE_DIR)/bin/$(APP) $(SYS_SBIN_DIR)/ -rf
	@cp $(PACKAGE_DIR)/bin/$(APP_PCAP_REPLAY) $(SYS_SBIN_DIR)/ -rf
	@echo " install finish!"

uninstall :
	@echo " uninstall DAPP..."
	@rm $(SYS_INSTALL_DIR) -rf
	@rm $(SYS_SBIN_DIR)/$(APP) -rf
	@rm $(SYS_SBIN_DIR)/$(APP_PCAP_REPLAY) -rf
	@echo " uninstall finish!"