HELIUM_IMG := $(OUT_DIR)helium.img

$(HELIUM_IMG): $(BOOTBOOT_CFG) $(BOOTIMG_CFG) $(KERNEL_BIN) $(INITRD_SYSROOT) \
			$(HOST_SYSROOT) $(MKBOOTIMG_BIN) $(FONT_FILE)
	$(MKDIR) -p $(dir $@)
	$(MKBOOTIMG_BIN) $(BOOTIMG_CFG) $@

.PHONY: bootimg
bootimg: $(HELIUM_IMG)
