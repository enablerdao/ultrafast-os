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

# Makefileを作成
echo -e "${YELLOW}Makefileを作成しています...${NC}"
cat > Makefile << 'EOF'
# macOS用Makefile

# コンパイラとフラグ
CC = gcc
CFLAGS = -Wall -Wextra -pthread

# ターゲット
all: os_simulator_mac fast_os_simulation_mac

# 日本語版シミュレータ
os_simulator_mac: os_simulator_mac.c
        $(CC) $(CFLAGS) -o $@ $<

# 英語版シミュレータ
fast_os_simulation_mac: fast_os_simulation_mac.c
        $(CC) $(CFLAGS) -o $@ $<

# クリーンアップ
clean:
        rm -f os_simulator_mac fast_os_simulation_mac

# 実行
run-jp: os_simulator_mac
        ./os_simulator_mac

run-en: fast_os_simulation_mac
        ./fast_os_simulation_mac

.PHONY: all clean run-jp run-en
EOF

# READMEを作成
echo -e "${YELLOW}READMEを作成しています...${NC}"
cat > README.md << 'EOF'
# 超高速OS シミュレータ (macOS版)

このプロジェクトは、高速起動と応答性を重視したオペレーティングシステムのシミュレータです。

## 実行方法

### 日本語版
```bash
./os_simulator_mac
```
または
```bash
make run-jp
```

### 英語版
```bash
./fast_os_simulation_mac
```
または
```bash
make run-en
```

## 特徴

- 1秒以内の起動時間
- 50ミリ秒以内の画面表示
- マイクロカーネルアーキテクチャ
- 効率的なメモリ管理
- 優先度ベースのプロセススケジューリング
- 並列タスク実行

## ライセンス

このプロジェクトは教育目的で自由に使用・改変可能です。
EOF

# コンパイル
echo -e "${YELLOW}シミュレータをコンパイルしています...${NC}"
gcc -Wall -Wextra -pthread -o os_simulator_mac os_simulator_mac.c
gcc -Wall -Wextra -pthread -o fast_os_simulation_mac fast_os_simulation_mac.c

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