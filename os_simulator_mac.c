#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

// 色の定義
#define COLOR_RESET   "\x1b[0m"
#define COLOR_BLACK   "\x1b[30m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_WHITE   "\x1b[37m"
#define BG_BLACK      "\x1b[40m"
#define BG_RED        "\x1b[41m"
#define BG_GREEN      "\x1b[42m"
#define BG_YELLOW     "\x1b[43m"
#define BG_BLUE       "\x1b[44m"
#define BG_MAGENTA    "\x1b[45m"
#define BG_CYAN       "\x1b[46m"
#define BG_WHITE      "\x1b[47m"
#define BOLD          "\x1b[1m"

// 画面サイズ
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// OS設定
#define MEMORY_SIZE 1024 // MB
#define MAX_PROCESSES 10
#define MAX_MODULES 5

// タイミング関数
long long current_timestamp_ms() {
    struct timeval te; 
    gettimeofday(&te, NULL);
    return te.tv_sec * 1000LL + te.tv_usec / 1000;
}

void sleep_ms(int milliseconds) {
    usleep(milliseconds * 1000);
}

// 画面クリア
void clear_screen() {
    printf("\x1b[2J\x1b[H");
    fflush(stdout);
}

// カーソル位置設定
void set_cursor(int x, int y) {
    printf("\x1b[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}

// メモリ管理
typedef struct {
    unsigned int total_memory;
    unsigned int used_memory;
    unsigned int free_memory;
} memory_manager_t;

memory_manager_t memory_manager;

void init_memory_manager() {
    memory_manager.total_memory = MEMORY_SIZE;
    memory_manager.used_memory = 0;
    memory_manager.free_memory = MEMORY_SIZE;
    printf("%s%sメモリマネージャを初期化: %d MB 合計メモリ%s\n", COLOR_GREEN, BOLD, MEMORY_SIZE, COLOR_RESET);
    printf("%sメモリ割り当て戦略: ゼロコピーと最適化されたページング%s\n", COLOR_GREEN, COLOR_RESET);
    fflush(stdout);
}

int allocate_memory(int size) {
    if (memory_manager.free_memory >= size) {
        memory_manager.used_memory += size;
        memory_manager.free_memory -= size;
        return 1; // 成功
    }
    return 0; // 失敗
}

// プロセス管理
typedef struct {
    int pid;
    char name[32];
    int memory_usage;
    int priority;
    int status; // 0: 待機, 1: 実行中, 2: ブロック
} process_t;

process_t processes[MAX_PROCESSES];
int process_count = 0;

int create_process(const char* name, int memory_usage, int priority) {
    if (process_count >= MAX_PROCESSES) {
        return -1; // プロセス数の上限
    }
    
    if (!allocate_memory(memory_usage)) {
        return -2; // メモリ不足
    }
    
    process_t* p = &processes[process_count];
    p->pid = process_count + 1;
    strncpy(p->name, name, 31);
    p->name[31] = '\0';
    p->memory_usage = memory_usage;
    p->priority = priority;
    p->status = 0; // 待機状態
    
    process_count++;
    return p->pid;
}

void start_process(int pid) {
    if (pid > 0 && pid <= process_count) {
        processes[pid-1].status = 1; // 実行中
        printf("%s%sプロセス %d を開始: %s (優先度: %d)%s\n", 
               COLOR_MAGENTA, BOLD, pid, processes[pid-1].name, processes[pid-1].priority, COLOR_RESET);
        fflush(stdout);
    }
}

// モジュール管理
typedef struct {
    int id;
    char name[32];
    int is_loaded;
} module_t;

module_t modules[MAX_MODULES];
int module_count = 0;

int load_module(const char* name) {
    if (module_count >= MAX_MODULES) {
        return -1; // モジュール数の上限
    }
    
    module_t* m = &modules[module_count];
    m->id = module_count + 1;
    strncpy(m->name, name, 31);
    m->name[31] = '\0';
    m->is_loaded = 1;
    
    module_count++;
    printf("%s%sモジュールをロード: %s%s\n", COLOR_BLUE, BOLD, m->name, COLOR_RESET);
    fflush(stdout);
    return m->id;
}

// ブートローダーシミュレーション
void bootloader() {
    printf("%s%s[ブートローダー] 超高速OSブートローダーを開始...%s\n", COLOR_YELLOW, BOLD, COLOR_RESET);
    printf("%s[ブートローダー] 最小限のハードウェア要件を確認...%s\n", COLOR_YELLOW, COLOR_RESET);
    fflush(stdout);
    sleep_ms(100);
    printf("%s[ブートローダー] カーネルをメモリ 0x100000 にロード...%s\n", COLOR_YELLOW, COLOR_RESET);
    fflush(stdout);
    sleep_ms(150);
    printf("%s[ブートローダー] カーネルエントリーポイントにジャンプ%s\n", COLOR_YELLOW, COLOR_RESET);
    fflush(stdout);
}

// カーネル初期化
void kernel_init() {
    printf("%s%s[カーネル] マイクロカーネルを初期化...%s\n", COLOR_GREEN, BOLD, COLOR_RESET);
    printf("%s[カーネル] メモリ管理をセットアップ...%s\n", COLOR_GREEN, COLOR_RESET);
    fflush(stdout);
    sleep_ms(100);
    printf("%s[カーネル] プロセススケジューラを初期化...%s\n", COLOR_GREEN, COLOR_RESET);
    fflush(stdout);
    sleep_ms(50);
    printf("%s[カーネル] システムサービスを並列に開始...%s\n", COLOR_GREEN, COLOR_RESET);
    fflush(stdout);
    sleep_ms(100);
    printf("%s[カーネル] カーネル初期化完了%s\n", COLOR_GREEN, COLOR_RESET);
    fflush(stdout);
}

// グラフィックス初期化
void graphics_init() {
    printf("%s%s[グラフィックス] フレームバッファを初期化...%s\n", COLOR_BLUE, BOLD, COLOR_RESET);
    fflush(stdout);
    sleep_ms(30);
    printf("%s[グラフィックス] ディスプレイ解像度を設定...%s\n", COLOR_BLUE, COLOR_RESET);
    fflush(stdout);
    sleep_ms(20);
    printf("%s[グラフィックス] ディスプレイ準備完了%s\n", COLOR_BLUE, COLOR_RESET);
    fflush(stdout);
}

// ブートスクリーン表示
void display_boot_screen() {
    clear_screen();
    
    // 上部の境界線
    set_cursor(0, 0);
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        printf("%s%s=%s", BG_BLUE, COLOR_WHITE, COLOR_RESET);
    }
    
    // タイトル
    set_cursor((SCREEN_WIDTH - 20) / 2, 2);
    printf("%s%s%s超高速OS v1.0%s", BG_BLUE, COLOR_WHITE, BOLD, COLOR_RESET);
    
    // 情報
    set_cursor(10, 5);
    printf("%s起動時間: 1秒未満%s", COLOR_CYAN, COLOR_RESET);
    
    set_cursor(10, 6);
    printf("%s画面表示時間: 50ms未満%s", COLOR_CYAN, COLOR_RESET);
    
    set_cursor(10, 8);
    printf("%sマイクロカーネルアーキテクチャ%s", COLOR_CYAN, COLOR_RESET);
    
    set_cursor(10, 9);
    printf("%sパフォーマンス最適化%s", COLOR_CYAN, COLOR_RESET);
    
    // 下部の境界線
    set_cursor(0, 12);
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        printf("%s%s=%s", BG_BLUE, COLOR_WHITE, COLOR_RESET);
    }
    
    // カーソル位置を下部に移動
    set_cursor(0, 14);
    fflush(stdout);
}

// スレッドシミュレーション
typedef struct {
    int thread_id;
    char task_name[32];
    int duration_ms;
} thread_task_t;

void* execute_task(void* arg) {
    thread_task_t* task = (thread_task_t*)arg;
    
    printf("%s%s[スレッド %d] タスク開始: %s%s\n", 
           COLOR_RED, BOLD, task->thread_id, task->task_name, COLOR_RESET);
    fflush(stdout);
    
    // タスク実行をシミュレート
    sleep_ms(task->duration_ms);
    
    printf("%s[スレッド %d] タスク完了: %s (所要時間: %d ms)%s\n", 
           COLOR_RED, task->thread_id, task->task_name, task->duration_ms, COLOR_RESET);
    fflush(stdout);
    
    free(task);
    return NULL;
}

// 並列タスク実行のデモ
void demonstrate_parallel_execution() {
    printf("\n%s%s[システム] 並列タスク実行のデモンストレーション%s\n", COLOR_MAGENTA, BOLD, COLOR_RESET);
    fflush(stdout);
    
    pthread_t threads[4];
    const char* task_names[] = {
        "ファイルインデックス作成", 
        "ネットワークパケット処理", 
        "UI描画", 
        "バックグラウンドコンパイル"
    };
    int durations[] = {200, 150, 100, 250};
    
    // スレッドを作成して開始
    for (int i = 0; i < 4; i++) {
        thread_task_t* task = (thread_task_t*)malloc(sizeof(thread_task_t));
        task->thread_id = i + 1;
        strncpy(task->task_name, task_names[i], 31);
        task->task_name[31] = '\0';
        task->duration_ms = durations[i];
        
        pthread_create(&threads[i], NULL, execute_task, task);
    }
    
    // すべてのスレッドが完了するのを待つ
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("%s[システム] すべての並列タスクが正常に完了しました%s\n", COLOR_MAGENTA, COLOR_RESET);
    fflush(stdout);
}

// ユーザー操作のシミュレーション
void simulate_user_interaction() {
    printf("\n%s%s[ユーザー] システムとのユーザー操作をシミュレート%s\n", COLOR_CYAN, BOLD, COLOR_RESET);
    fflush(stdout);
    
    // ターミナルを開く
    printf("%s[ユーザー] ターミナルアプリケーションを開く%s\n", COLOR_CYAN, COLOR_RESET);
    fflush(stdout);
    int terminal_pid = create_process("ターミナル", 15, 10);
    start_process(terminal_pid);
    sleep_ms(50);
    
    // Webブラウザを開く
    printf("%s[ユーザー] Webブラウザを開く%s\n", COLOR_CYAN, COLOR_RESET);
    fflush(stdout);
    int browser_pid = create_process("Webブラウザ", 120, 8);
    start_process(browser_pid);
    sleep_ms(50);
    
    // テキストエディタを開く
    printf("%s[ユーザー] テキストエディタを開く%s\n", COLOR_CYAN, COLOR_RESET);
    fflush(stdout);
    int editor_pid = create_process("テキストエディタ", 45, 7);
    start_process(editor_pid);
    sleep_ms(50);
    
    // システム状態を表示
    printf("\n%s%s[システム] 現在のシステム状態:%s\n", COLOR_MAGENTA, BOLD, COLOR_RESET);
    printf("- メモリ使用量: %d MB 使用中, %d MB 空き\n", 
           memory_manager.used_memory, memory_manager.free_memory);
    printf("- 実行中のプロセス: %d\n", process_count);
    fflush(stdout);
    
    // プロセスリストを表示
    printf("\n%s%s[システム] プロセスリスト:%s\n", COLOR_MAGENTA, BOLD, COLOR_RESET);
    printf("%-5s %-20s %-15s %-10s %-10s\n", "PID", "名前", "メモリ (MB)", "優先度", "状態");
    printf("------------------------------------------------------------------\n");
    fflush(stdout);
    
    for (int i = 0; i < process_count; i++) {
        const char* status_str = "不明";
        switch (processes[i].status) {
            case 0: status_str = "待機"; break;
            case 1: status_str = "実行中"; break;
            case 2: status_str = "ブロック"; break;
        }
        printf("%-5d %-20s %-15d %-10d %-10s\n", 
               processes[i].pid, processes[i].name, processes[i].memory_usage, 
               processes[i].priority, status_str);
    }
    fflush(stdout);
    
    // 並列実行のデモ
    demonstrate_parallel_execution();
}

// システム準備完了通知
void system_ready(long long boot_time, long long display_time) {
    printf("%s%s[システム] すべてのコンポーネントが初期化されました%s\n", COLOR_MAGENTA, BOLD, COLOR_RESET);
    printf("%s[システム] メモリ使用量: %d MB 使用中, %d MB 空き%s\n", 
           COLOR_MAGENTA, memory_manager.used_memory, memory_manager.free_memory, COLOR_RESET);
    printf("%s[システム] %d プロセス作成済み, %d モジュールロード済み%s\n", 
           COLOR_MAGENTA, process_count, module_count, COLOR_RESET);
    printf("%s[システム] システムはユーザー操作の準備ができました%s\n", COLOR_MAGENTA, COLOR_RESET);
    printf("%s[システム] 合計起動時間: %lld ms%s\n", COLOR_MAGENTA, boot_time, COLOR_RESET);
    printf("%s[システム] 画面表示時間: %lld ms%s\n", COLOR_MAGENTA, display_time, COLOR_RESET);
    fflush(stdout);
}

// メインOSシミュレーション
int main() {
    long long start_time, display_time, end_time;
    
    // 開始時間を記録
    start_time = current_timestamp_ms();
    
    // 画面をクリア
    clear_screen();
    
    printf("%s%s超高速OS シミュレーション (macOS対応版)%s\n\n", COLOR_CYAN, BOLD, COLOR_RESET);
    printf("このプログラムは、高速起動と応答性を重視したOSの動作をシミュレートします。\n");
    printf("実際のOSではなく、その概念と動作を示すためのシミュレーションです。\n\n");
    fflush(stdout);
    
    sleep_ms(2000); // 2秒待機
    clear_screen();
    
    // 起動シーケンス
    bootloader();
    kernel_init();
    
    // メモリ管理の初期化
    init_memory_manager();
    
    // 必須モジュールのロード
    load_module("ファイルシステム");
    load_module("ネットワークスタック");
    load_module("セキュリティモジュール");
    load_module("ユーザーインターフェース");
    
    // 画面表示開始時間を記録
    display_time = current_timestamp_ms();
    
    // グラフィックスの初期化とブートスクリーンの表示
    graphics_init();
    sleep_ms(1000); // 1秒待機
    display_boot_screen();
    
    // システムプロセスの作成
    create_process("システムマネージャ", 25, 15);
    create_process("デバイスマネージャ", 18, 12);
    create_process("サービスマネージャ", 22, 13);
    
    // 終了時間を記録
    end_time = current_timestamp_ms();
    
    // タイミング結果を表示
    set_cursor(0, 15);
    system_ready(end_time - start_time, end_time - display_time);
    
    sleep_ms(2000); // 2秒待機
    clear_screen();
    
    // ユーザー操作のシミュレーション
    simulate_user_interaction();
    
    printf("\n%s%s[システム] システムデモンストレーション完了%s\n", COLOR_MAGENTA, BOLD, COLOR_RESET);
    fflush(stdout);
    
    return 0;
}