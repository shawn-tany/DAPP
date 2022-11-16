ROOT_DIR = $(PWD)
SYS_SBIN_DIR = /usr/sbin
SYS_INSTALL_DIR = /etc/DAPP
PACKAGE_DIR = $(ROOT_DIR)/package

ALL : SOURCE_ALL TOOLS_ALL

clean : source_clean tools_clean

include source/Makefile
include tools/Makefile

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
	@cp $(TOOLS_DIR)/build/bin/* $(PACKAGE_DIR)/tools/bin -rf
	@cp $(TOOLS_DIR)/*.sh $(PACKAGE_DIR)/tools -rf
	@cp $(TOOLS_DIR)/*.py $(PACKAGE_DIR)/tools -rf

	@# Create Package
	@rm $(PACKAGE_DIR)/$(PACKAGE) -rf
	@tar -zcPf $(PACKAGE_DIR)/$(PACKAGE) $(PACKAGE_DIR)/*

	@# Install to system directory
	@cp $(PACKAGE_DIR)/* $(SYS_INSTALL_DIR) -rf
	@cp $(PACKAGE_DIR)/bin/$(APP) $(SYS_SBIN_DIR)
	@cp $(PACKAGE_DIR)/tools/bin/$(APP_PCAP_REPLAY) $(SYS_SBIN_DIR)
	@echo " install finish!"

unstall :
	@echo " unstall DAPP..."
	@rm $(SYS_INSTALL_DIR) -rf
	@rm $(SYS_SBIN_DIR)/$(APP) -rf
	@echo " unstall finish!"