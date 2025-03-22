# 超高速OS シミュレータ (Docker版)

このプロジェクトは、高速起動と応答性を重視したオペレーティングシステムのシミュレータをDockerコンテナで実行できるようにしたものです。

## 特徴

- 1秒以内の起動時間
- 50ミリ秒以内の画面表示
- マイクロカーネルアーキテクチャ
- 効率的なメモリ管理
- 優先度ベースのプロセススケジューリング
- 並列タスク実行

## 実行方法

### 前提条件

- Docker
- Docker Compose

### ビルドと実行

```bash
# リポジトリをクローン
git clone https://github.com/enablerdao/ultrafast-os.git
cd ultrafast-os/docker

# Dockerイメージをビルドして実行
docker-compose up
```

または、個別のコンテナを実行することもできます：

```bash
# 日本語版
docker-compose up ultrafast-os-jp

# 英語版
docker-compose up ultrafast-os-en
```

### 手動ビルド

```bash
# Dockerイメージをビルド
docker build -t ultrafast-os .

# 日本語版を実行
docker run -it --name ultrafast-os-jp ultrafast-os ./os_simulator_auto

# 英語版を実行
docker run -it --name ultrafast-os-en ultrafast-os ./fast_os_simulation
```

## プロジェクトの構成

- `os_simulator.c` - 日本語版OSシミュレータ
- `fast_os_simulation.c` - 英語版OSシミュレータ
- `Makefile` - ビルド設定
- `Dockerfile` - Dockerイメージ設定
- `docker-compose.yml` - Docker Compose設定

## ライセンス

このプロジェクトは教育目的で自由に使用・改変可能です。