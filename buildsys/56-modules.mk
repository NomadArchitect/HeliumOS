
MODULE_LD_TEMPLATE := $(TEMPLATES_DIR)module.ld
MODULE_LINKSCRIPT := $(BUILD_DIR)module.ld

$(MODULE_LINKSCRIPT): $(KERNEL_BIN) $(MODLDGEN_BIN) $(MODULE_LD_TEMPLATE)
	$(MODLDGEN_BIN) $(KERNEL_SYM) $@
	$(PY) $(PYDIR)modldgen.py $(MODULE_LD_TEMPLATE) $@

.PHONY: modld
modld: $(MODULE_LINKSCRIPT)

CLEAN += $(INITRD_SYSROOT)modules/

MODULES_ALL :=