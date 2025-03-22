FROM ubuntu:22.04

# 必要なパッケージをインストール
RUN apt-get update && apt-get install -y \
    build-essential \
    nasm \
    qemu-system-x86 \
    xorriso \
    grub-pc-bin \
    grub-common \
    mtools \
    gcc-multilib \
    git \
    && rm -rf /var/lib/apt/lists/*

# 作業ディレクトリを設定
WORKDIR /ultrafast-os

# ソースコードをコピー
COPY bootloader.asm /ultrafast-os/
COPY kernel_entry.asm /ultrafast-os/
COPY kernel.c /ultrafast-os/
COPY linker.ld /ultrafast-os/

# ビルドスクリプトをコピー
COPY build.sh /ultrafast-os/
RUN chmod +x /ultrafast-os/build.sh

# 起動スクリプトをコピー
COPY run.sh /ultrafast-os/
RUN chmod +x /ultrafast-os/run.sh

# デフォルトコマンド
CMD ["/bin/bash"]