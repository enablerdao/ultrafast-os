# Ultra Fast OS Makefile

# コンパイラとツール
ASM = nasm
CC = gcc
LD = ld
QEMU = qemu-system-i386

# フラグとオプション
ASMFLAGS = -f bin
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -m elf_i386 -T linker.ld

# ソースファイル
BOOTLOADER_SRC = bootloader.asm
KERNEL_ENTRY_SRC = kernel_entry.asm
KERNEL_SRC = kernel.c

# 出力ファイル
BOOTLOADER_BIN = bootloader.bin
KERNEL_ENTRY_BIN = kernel_entry.bin
KERNEL_OBJ = kernel.o
KERNEL_BIN = kernel.bin
OS_IMAGE = os_image.bin

# デフォルトターゲット
all: $(OS_IMAGE)

# ブートローダーのコンパイル
$(BOOTLOADER_BIN): $(BOOTLOADER_SRC)
	$(ASM) $(ASMFLAGS) $< -o $@

# カーネルエントリーのコンパイル
$(KERNEL_ENTRY_BIN): $(KERNEL_ENTRY_SRC)
	$(ASM) $(ASMFLAGS) $< -o $@

# カーネルのコンパイル
$(KERNEL_OBJ): $(KERNEL_SRC)
	$(CC) $(CFLAGS) $< -o $@

# カーネルのリンク
$(KERNEL_BIN): $(KERNEL_OBJ)
	$(LD) $(LDFLAGS) -o $@ $<

# OSイメージの作成
$(OS_IMAGE): $(BOOTLOADER_BIN) $(KERNEL_ENTRY_BIN) $(KERNEL_BIN)
	cat $(BOOTLOADER_BIN) $(KERNEL_ENTRY_BIN) $(KERNEL_BIN) > $(OS_IMAGE)
	# パディングを追加して1.44MBのフロッピーディスクサイズにする
	dd if=/dev/zero bs=1024 count=1440 >> $(OS_IMAGE)
	truncate -s 1474560 $(OS_IMAGE)

# QEMUで実行
run: $(OS_IMAGE)
	$(QEMU) -fda $(OS_IMAGE)

# クリーンアップ
clean:
	rm -f $(BOOTLOADER_BIN) $(KERNEL_ENTRY_BIN) $(KERNEL_OBJ) $(KERNEL_BIN) $(OS_IMAGE)

.PHONY: all run clean