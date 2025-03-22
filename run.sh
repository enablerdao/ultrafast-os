#!/bin/bash

# 色の定義
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${CYAN}======================================================${NC}"
echo -e "${CYAN}       超高速OS 実行スクリプト                       ${NC}"
echo -e "${CYAN}======================================================${NC}"
echo

# ISOイメージが存在するか確認
if [ ! -f "ultrafast-os.iso" ]; then
    echo -e "${YELLOW}ISOイメージが見つかりません。ビルドを実行します...${NC}"
    ./build.sh
fi

echo -e "${YELLOW}QEMUでOSを起動しています...${NC}"
qemu-system-i386 -cdrom ultrafast-os.iso -m 128

echo -e "${GREEN}実行完了！${NC}"
echo
echo -e "${CYAN}======================================================${NC}"
echo -e "${CYAN}                実行完了                             ${NC}"
echo -e "${CYAN}======================================================${NC}"