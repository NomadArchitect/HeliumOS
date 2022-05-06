M ?= DEBUG

QEMU_BIN   := qemu-system-x86_64
OVMF_CODE  := /usr/share/edk2-ovmf/x64/OVMF_CODE.fd
OVMF_VARS  := /usr/share/edk2-ovmf/x64/OVMF_VARS.fd
QEMU_FLAGS := -cpu qemu64 -smp 1 -net none -m 64M

# Toolchain to build targets for the build machine
BUILD_CC := gcc

LSB_RELEASE := lsb_release

MKFIFO := mkfifo
UMOUNT := umount
MKDIR  := mkdir
MOUNT  := mount
TOUCH  := touch
ECHO   := echo
EXIT   := exit
SOCAT  := socat
TEST   := test
TMUX   := tmux
CURL   := curl
FIND   := find
GZIP   := gzip -f
MAKE   := make -j4
CAT    := cat
CUT    := cut
GDB    := gdb
GIT    := git
TAR    := tar
SED    := sed
CD 	   := cd
CP     := cp -rf
LS     := ls
MV     := mv -f
RM     := rm -rf
SH     := sh
TR     := tr

# if this has a problem detecting your distro, just put your distro name manually
DISTRO := $(shell $(LSB_RELEASE) -i | $(CUT) -d ':' -f2 | $(TR) -d " \t")

KERNEL_DIR := kernel/
KERNEL_SRC_DIR := $(KERNEL_DIR)/src/
KERNEL_INC_DIR := $(KERNEL_DIR)/include/
STD_INC        := $(KERNEL_DIR)/stdinc/
LINKER_SCRIPT  := $(KERNEL_DIR)/link.ld
FONT_FILE      := $(KERNEL_DIR)/font.psf

BOOTBOOT_CFG := $(KERNEL_DIR)/bootboot.config
BOOTIMG_CFG  := $(KERNEL_DIR)/bootimg.json

ROOT_DIR 	  := $(shell pwd -P)

EXT_DIR 	  := $(ROOT_DIR)/ext/
EXT_SRC_DIR   := $(EXT_DIR)/src/
EXT_BUILD_DIR := $(EXT_DIR)/build/

SYSROOTS 	   := $(ROOT_DIR)/sysroots/
INITRD_SYSROOT := $(SYSROOTS)/initrd/
BUILD_SYSROOT  := $(SYSROOTS)/build/
HOST_SYSROOT   := $(SYSROOTS)/host/

BUILD_DIR := $(ROOT_DIR)/build/

OUT_DIR := $(BUILD_DIR)/out/

BUILDSYS := buildsys/
SEDDIR   := $(BUILDSYS)/sedscripts/

CLEAN := build

.PHONY: nothing
nothing:
