FROM ubuntu:22.04

# 必要なパッケージをインストール
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    make \
    libncurses5-dev \
    && rm -rf /var/lib/apt/lists/*

# 作業ディレクトリを設定
WORKDIR /app

# ソースコードをコピー
COPY os_simulator.c /app/
COPY fast_os_simulation.c /app/
COPY Makefile /app/

# ビルド
RUN make

# 実行コマンド
CMD ["./os_simulator_auto"]