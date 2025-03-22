/* 
 * Ultra Fast OS - カーネル
 * 高速起動と応答性を重視したシンプルなOSカーネル
 */

// ビデオメモリのアドレス
#define VIDEO_MEMORY 0xB8000
// 画面の幅と高さ
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// 色の定義
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_LIGHT_GRAY 7
#define COLOR_DARK_GRAY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 10
#define COLOR_LIGHT_CYAN 11
#define COLOR_LIGHT_RED 12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

// 文字属性を作成（背景色と文字色）
#define MAKE_COLOR(bg, fg) ((bg << 4) | fg)
// 文字エントリを作成（文字と属性）
#define MAKE_ENTRY(c, color) ((color << 8) | c)

// カーソル位置
unsigned int cursor_x = 0;
unsigned int cursor_y = 0;

// メモリ管理の基本構造
typedef struct {
    unsigned int total_memory;
    unsigned int used_memory;
    unsigned int free_memory;
} memory_manager_t;

// メモリマネージャのインスタンス
memory_manager_t memory_manager;

// プロセス構造体
typedef struct {
    int pid;
    char name[32];
    int priority;
    int status; // 0: 待機, 1: 実行中, 2: ブロック
} process_t;

// プロセス管理
#define MAX_PROCESSES 10
process_t processes[MAX_PROCESSES];
int process_count = 0;

// モジュール構造体
typedef struct {
    int id;
    char name[32];
    int is_loaded;
} module_t;

// モジュール管理
#define MAX_MODULES 5
module_t modules[MAX_MODULES];
int module_count = 0;

// 画面をクリアする関数
void clear_screen() {
    unsigned short* video_memory = (unsigned short*)VIDEO_MEMORY;
    unsigned short blank = MAKE_ENTRY(' ', MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        video_memory[i] = blank;
    }
    
    cursor_x = 0;
    cursor_y = 0;
}

// 文字を画面に表示する関数
void put_char(char c, unsigned char color) {
    unsigned short* video_memory = (unsigned short*)VIDEO_MEMORY;
    
    // 改行の処理
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        return;
    }
    
    // 通常の文字を表示
    unsigned int index = cursor_y * SCREEN_WIDTH + cursor_x;
    video_memory[index] = MAKE_ENTRY(c, color);
    
    // カーソルを進める
    cursor_x++;
    if (cursor_x >= SCREEN_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    // 画面の下端に達した場合のスクロール処理（簡易版）
    if (cursor_y >= SCREEN_HEIGHT) {
        // 簡易的なスクロール：画面をクリアして先頭に戻る
        clear_screen();
    }
}

// 文字列を表示する関数
void print(const char* str, unsigned char color) {
    while (*str) {
        put_char(*str, color);
        str++;
    }
}

// 数値を16進数で表示する関数
void print_hex(unsigned int num, unsigned char color) {
    char hex_chars[] = "0123456789ABCDEF";
    char hex_str[11] = "0x00000000";
    
    for (int i = 9; i > 1; i--) {
        hex_str[i] = hex_chars[num & 0xF];
        num >>= 4;
    }
    
    print(hex_str, color);
}

// 数値を10進数で表示する関数
void print_dec(int num, unsigned char color) {
    char dec_str[12];
    int i = 0;
    int is_negative = 0;
    
    // 負の数の処理
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }
    
    // 0の特別処理
    if (num == 0) {
        dec_str[i++] = '0';
    }
    
    // 数字を文字列に変換
    while (num > 0) {
        dec_str[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    // 負の符号を追加
    if (is_negative) {
        dec_str[i++] = '-';
    }
    
    // 文字列を逆順にして表示
    while (i > 0) {
        put_char(dec_str[--i], color);
    }
}

// メモリ管理の初期化
void init_memory_manager(unsigned int total_memory) {
    memory_manager.total_memory = total_memory;
    memory_manager.used_memory = 0;
    memory_manager.free_memory = total_memory;
    
    print("Memory manager initialized: ", MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
    print_dec(total_memory / 1024, MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
    print(" KB total memory\n", MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
}

// メモリ割り当て（簡易版）
int allocate_memory(unsigned int size) {
    if (memory_manager.free_memory >= size) {
        memory_manager.used_memory += size;
        memory_manager.free_memory -= size;
        return 1; // 成功
    }
    return 0; // 失敗
}

// プロセスの作成
int create_process(const char* name, int priority) {
    if (process_count >= MAX_PROCESSES) {
        return -1; // プロセス数の上限
    }
    
    // メモリ割り当て（仮の値）
    if (!allocate_memory(64)) {
        return -2; // メモリ不足
    }
    
    process_t* p = &processes[process_count];
    p->pid = process_count + 1;
    
    // 名前のコピー
    int i;
    for (i = 0; name[i] && i < 31; i++) {
        p->name[i] = name[i];
    }
    p->name[i] = '\0';
    
    p->priority = priority;
    p->status = 0; // 待機状態
    
    process_count++;
    return p->pid;
}

// プロセスの開始
void start_process(int pid) {
    if (pid > 0 && pid <= process_count) {
        processes[pid-1].status = 1; // 実行中
        
        print("Started process ", MAKE_COLOR(COLOR_BLACK, COLOR_MAGENTA));
        print_dec(pid, MAKE_COLOR(COLOR_BLACK, COLOR_MAGENTA));
        print(": ", MAKE_COLOR(COLOR_BLACK, COLOR_MAGENTA));
        print(processes[pid-1].name, MAKE_COLOR(COLOR_BLACK, COLOR_MAGENTA));
        print("\n", MAKE_COLOR(COLOR_BLACK, COLOR_MAGENTA));
    }
}

// モジュールのロード
int load_module(const char* name) {
    if (module_count >= MAX_MODULES) {
        return -1; // モジュール数の上限
    }
    
    module_t* m = &modules[module_count];
    m->id = module_count + 1;
    
    // 名前のコピー
    int i;
    for (i = 0; name[i] && i < 31; i++) {
        m->name[i] = name[i];
    }
    m->name[i] = '\0';
    
    m->is_loaded = 1;
    
    print("Loaded module: ", MAKE_COLOR(COLOR_BLACK, COLOR_BLUE));
    print(m->name, MAKE_COLOR(COLOR_BLACK, COLOR_BLUE));
    print("\n", MAKE_COLOR(COLOR_BLACK, COLOR_BLUE));
    
    module_count++;
    return m->id;
}

// ブートスクリーンの表示
void display_boot_screen() {
    unsigned char title_color = MAKE_COLOR(COLOR_BLUE, COLOR_WHITE);
    unsigned char border_color = MAKE_COLOR(COLOR_BLACK, COLOR_CYAN);
    unsigned char text_color = MAKE_COLOR(COLOR_BLACK, COLOR_WHITE);
    
    // 上部の境界線
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        put_char('=', border_color);
    }
    
    // タイトル
    cursor_x = (SCREEN_WIDTH - 20) / 2;
    cursor_y = 2;
    print("ULTRA FAST OS v1.0", title_color);
    
    // 情報
    cursor_x = 10;
    cursor_y = 5;
    print("Boot time: < 1 second", text_color);
    
    cursor_x = 10;
    cursor_y = 6;
    print("Display time: < 50ms", text_color);
    
    cursor_x = 10;
    cursor_y = 8;
    print("Microkernel Architecture", text_color);
    
    cursor_x = 10;
    cursor_y = 9;
    print("Optimized for Performance", text_color);
    
    // 下部の境界線
    cursor_x = 0;
    cursor_y = 12;
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        put_char('=', border_color);
    }
    
    // カーソル位置を下部に移動
    cursor_x = 0;
    cursor_y = 14;
}

// システム情報の表示
void show_system_info() {
    unsigned char info_color = MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_GREEN);
    
    print("System Information:\n", info_color);
    
    print("- Memory usage: ", info_color);
    print_dec(memory_manager.used_memory / 1024, info_color);
    print(" KB used, ", info_color);
    print_dec(memory_manager.free_memory / 1024, info_color);
    print(" KB free\n", info_color);
    
    print("- Processes: ", info_color);
    print_dec(process_count, info_color);
    print("\n", info_color);
    
    print("- Modules: ", info_color);
    print_dec(module_count, info_color);
    print("\n", info_color);
}

// プロセスリストの表示
void show_process_list() {
    unsigned char header_color = MAKE_COLOR(COLOR_BLACK, COLOR_YELLOW);
    unsigned char data_color = MAKE_COLOR(COLOR_BLACK, COLOR_WHITE);
    
    print("\nProcess List:\n", header_color);
    print("PID  Name                 Priority  Status\n", header_color);
    print("------------------------------------------\n", header_color);
    
    for (int i = 0; i < process_count; i++) {
        // PID
        print_dec(processes[i].pid, data_color);
        
        // 名前の表示（パディング付き）
        cursor_x = 5;
        print(processes[i].name, data_color);
        
        // 優先度
        cursor_x = 27;
        print_dec(processes[i].priority, data_color);
        
        // ステータス
        cursor_x = 36;
        if (processes[i].status == 0) {
            print("Idle", data_color);
        } else if (processes[i].status == 1) {
            print("Running", data_color);
        } else if (processes[i].status == 2) {
            print("Blocked", data_color);
        }
        
        print("\n", data_color);
    }
}

// カーネルのメイン関数
void kernel_main() {
    // 画面をクリア
    clear_screen();
    
    // 起動メッセージ
    print("Ultra Fast OS Kernel starting...\n", MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
    
    // メモリ管理の初期化（仮の値: 64MB）
    init_memory_manager(64 * 1024);
    
    // 基本モジュールのロード
    load_module("FileSystem");
    load_module("NetworkStack");
    load_module("SecurityModule");
    load_module("UserInterface");
    
    // ブートスクリーンの表示
    display_boot_screen();
    
    // システムプロセスの作成
    create_process("SystemManager", 15);
    create_process("DeviceManager", 12);
    create_process("ServiceManager", 13);
    
    // プロセスの開始
    start_process(1); // SystemManager
    start_process(2); // DeviceManager
    start_process(3); // ServiceManager
    
    // ユーザープロセスの作成と開始
    int terminal_pid = create_process("Terminal", 10);
    start_process(terminal_pid);
    
    int browser_pid = create_process("WebBrowser", 8);
    start_process(browser_pid);
    
    int editor_pid = create_process("TextEditor", 7);
    start_process(editor_pid);
    
    // システム情報の表示
    show_system_info();
    
    // プロセスリストの表示
    show_process_list();
    
    // 完了メッセージ
    print("\nSystem initialization complete!\n", MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_MAGENTA));
    
    // カーネルの処理はここで終了（実際のOSではここで無限ループになる）
}

// カーネルのエントリーポイント（アセンブリから呼び出される）
void _start() {
    kernel_main();
    
    // 無限ループ（実際のOSではここに到達しない）
    while (1) {
        // 何もしない
    }
}