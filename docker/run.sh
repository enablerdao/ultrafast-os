#!/bin/bash

# 色の定義
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${CYAN}======================================================${NC}"
echo -e "${CYAN}       超高速OS シミュレータ 実行スクリプト           ${NC}"
echo -e "${CYAN}======================================================${NC}"
echo

# Dockerがインストールされているか確認
if ! command -v docker &> /dev/null; then
    echo -e "${YELLOW}Dockerがインストールされていません。${NC}"
    echo -e "${YELLOW}以下のURLからDockerをインストールしてください：${NC}"
    echo -e "https://docs.docker.com/get-docker/"
    exit 1
fi

# Docker Composeがインストールされているか確認
if ! command -v docker-compose &> /dev/null; then
    echo -e "${YELLOW}Docker Composeがインストールされていません。${NC}"
    echo -e "${YELLOW}以下のURLからDocker Composeをインストールしてください：${NC}"
    echo -e "https://docs.docker.com/compose/install/"
    exit 1
fi

echo -e "${GREEN}Dockerが見つかりました。${NC}"

# 言語選択
echo -e "${YELLOW}実行する言語を選択してください:${NC}"
echo -e "1) 日本語"
echo -e "2) 英語"
echo -e "3) 両方"
read -p "選択 (1-3): " lang_choice

case $lang_choice in
    1)
        echo -e "${GREEN}日本語版を実行します...${NC}"
        docker-compose up ultrafast-os-jp
        ;;
    2)
        echo -e "${GREEN}英語版を実行します...${NC}"
        docker-compose up ultrafast-os-en
        ;;
    3)
        echo -e "${GREEN}両方のバージョンを実行します...${NC}"
        docker-compose up
        ;;
    *)
        echo -e "${YELLOW}無効な選択です。日本語版を実行します...${NC}"
        docker-compose up ultrafast-os-jp
        ;;
esac

echo -e "${CYAN}======================================================${NC}"
echo -e "${CYAN}                実行完了                             ${NC}"
echo -e "${CYAN}======================================================${NC}"