# 超高速OS プロジェクト

このプロジェクトは、高速起動と応答性を重視したオペレーティングシステムの概念実装です。1秒以内の起動時間と50ミリ秒以内の画面表示を目標としています。

## 実行方法

### Docker を使用した実行方法（推奨）

最も簡単な方法は、Docker を使用してシミュレータを実行することです：

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

### 手動でのコンパイルと実行

```bash
# 必要なパッケージをインストール
sudo apt-get update
sudo apt-get install -y build-essential gcc make libncurses5-dev

# リポジトリをクローン
git clone https://github.com/enablerdao/ultrafast-os.git
cd ultrafast-os

# シミュレータをコンパイル
gcc -o os_simulator_auto os_simulator_auto.c -pthread
gcc -o fast_os_simulation fast_os_simulation.c -pthread

# 実行
./os_simulator_auto  # 日本語版
./fast_os_simulation # 英語版
```

### macOS での実行方法

macOS用のインストールスクリプトを使用できます：

```bash
# リポジトリをクローン
git clone https://github.com/enablerdao/ultrafast-os.git
cd ultrafast-os

# インストールスクリプトを実行
chmod +x install_mac.sh
./install_mac.sh
```

## プロジェクトの構成

このプロジェクトは以下のファイルで構成されています：

1. **シミュレータ**:
   - `os_simulator_auto.c` - 日本語版OSシミュレータ
   - `fast_os_simulation.c` - 英語版OSシミュレータ
   - `docker/` - Dockerベースのシミュレータ

2. **OS実装コンセプト**:
   - `bootloader.asm` - ブートローダーのアセンブリコード
   - `kernel_entry.asm` - カーネルエントリーポイントのアセンブリコード
   - `kernel.c` - カーネルのCコード
   - `kernel_64.c` - 64ビット用カーネルのCコード
   - `linker.ld` - リンカスクリプト

## 主要な機能

### 1. 高速起動
- 最小限のハードウェアチェック
- 必要なコンポーネントのみを初期化
- 並列初期化プロセス

### 2. メモリ管理
- ゼロコピー技術
- 最適化されたページング
- 効率的なメモリ割り当て

### 3. プロセス管理
- 優先度ベースのスケジューリング
- 高速プロセス作成と切り替え
- リアルタイム応答性

### 4. モジュール式設計
- 必要に応じて動的にロードされるモジュール
- 明確に定義されたインターフェース
- 開発者フレンドリーな構造

### 5. 並列処理
- マルチスレッド対応
- 効率的なタスク実行
- CPUコアの最大活用

## 設計思想

このOSは以下の設計思想に基づいています：

1. **最小主義**: 必要最小限の機能のみを実装
2. **高速性**: すべての操作で速度を最優先
3. **モジュール性**: 明確に分離されたコンポーネント
4. **開発者フレンドリー**: 理解しやすく拡張可能な設計
5. **ユーザビリティ**: 高速な応答性と直感的な操作

## 今後の展望

- **ドライバフレームワーク**: 様々なハードウェアをサポート
- **ファイルシステム**: 高速なデータアクセス
- **ネットワークスタック**: 効率的な通信
- **セキュリティ機能**: 堅牢なシステム保護
- **グラフィカルインターフェース**: 高速で美しいUI

## 注意事項

このプロジェクトは教育目的で作成されたコンセプト実装です。実際の環境で動作するOSを構築するには、さらに多くの開発が必要です。

## ライセンス

このプロジェクトは教育目的で自由に使用・改変可能です。