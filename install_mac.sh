#!/bin/bash

# 超高速OSシミュレータ macOSインストールスクリプト

# 色の定義
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${CYAN}======================================================${NC}"
echo -e "${CYAN}       超高速OS シミュレータ macOSインストーラ         ${NC}"
echo -e "${CYAN}======================================================${NC}"
echo

# Command Line Toolsのチェック
if ! command -v gcc &> /dev/null; then
    echo -e "${YELLOW}Command Line Toolsがインストールされていません。${NC}"
    echo -e "${YELLOW}以下のコマンドを実行してインストールしてください：${NC}"
    echo -e "xcode-select --install"
    exit 1
fi

echo -e "${GREEN}Command Line Toolsが見つかりました。${NC}"

# ディレクトリの作成
echo -e "${YELLOW}インストール先ディレクトリを作成しています...${NC}"
mkdir -p ~/UltraFastOS
cd ~/UltraFastOS

# ファイルのコピー
echo -e "${YELLOW}ファイルをコピーしています...${NC}"

# 現在のスクリプトのディレクトリを取得
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# ファイルをコピー
cp "$SCRIPT_DIR/os_simulator_mac.c" .
cp "$SCRIPT_DIR/fast_os_simulation_mac.c" .
cp "$SCRIPT_DIR/Makefile.mac.fixed" ./Makefile
cp "$SCRIPT_DIR/README_MAC.md" ./README.md

# コンパイル
echo -e "${YELLOW}シミュレータをコンパイルしています...${NC}"
make

# 実行ファイルの権限設定
chmod +x os_simulator_mac fast_os_simulation_mac

echo -e "${GREEN}インストールが完了しました！${NC}"
echo -e "${YELLOW}インストール先: ${NC}~/UltraFastOS"
echo
echo -e "${CYAN}実行方法:${NC}"
echo -e "  日本語版: ${YELLOW}cd ~/UltraFastOS && ./os_simulator_mac${NC}"
echo -e "  英語版: ${YELLOW}cd ~/UltraFastOS && ./fast_os_simulation_mac${NC}"
echo
echo -e "${CYAN}または:${NC}"
echo -e "  日本語版: ${YELLOW}cd ~/UltraFastOS && make run-jp${NC}"
echo -e "  英語版: ${YELLOW}cd ~/UltraFastOS && make run-en${NC}"
echo
echo -e "${CYAN}======================================================${NC}"
echo -e "${CYAN}                インストール完了                      ${NC}"
echo -e "${CYAN}======================================================${NC}"