#!/bin/bash

# 色の定義
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${CYAN}======================================================${NC}"
echo -e "${CYAN}       超高速OS ビルドスクリプト                      ${NC}"
echo -e "${CYAN}======================================================${NC}"
echo

# 出力ディレクトリを作成
mkdir -p build
mkdir -p iso/boot/grub

# ブートローダーをコンパイル
echo -e "${YELLOW}ブートローダーをコンパイル中...${NC}"
nasm -f bin bootloader.asm -o build/bootloader.bin

# カーネルエントリーをコンパイル
echo -e "${YELLOW}カーネルエントリーをコンパイル中...${NC}"
nasm -f elf32 kernel_entry.asm -o build/kernel_entry.o

# カーネルをコンパイル
echo -e "${YELLOW}カーネルをコンパイル中...${NC}"
gcc -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -c kernel.c -o build/kernel.o

# カーネルをリンク
echo -e "${YELLOW}カーネルをリンク中...${NC}"
ld -m elf_i386 -T linker.ld -o build/kernel.bin build/kernel_entry.o build/kernel.o

# カーネルをISOイメージに組み込む
echo -e "${YELLOW}ISOイメージを作成中...${NC}"
cat > iso/boot/grub/grub.cfg << EOF
set timeout=0
set default=0

menuentry "UltraFast OS" {
    multiboot /boot/kernel.bin
    boot
}
EOF

cp build/kernel.bin iso/boot/

# ISOイメージを作成
grub-mkrescue -o ultrafast-os.iso iso

echo -e "${GREEN}ビルド完了！${NC}"
echo -e "${YELLOW}ISOイメージ: ${NC}ultrafast-os.iso"
echo
echo -e "${CYAN}======================================================${NC}"
echo -e "${CYAN}                ビルド完了                           ${NC}"
echo -e "${CYAN}======================================================${NC}"