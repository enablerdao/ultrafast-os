; シンプルなブートローダー
[BITS 16]       ; 16ビットモードで実行
[ORG 0x7C00]    ; ブートセクタは0x7C00にロードされる

; ブートローダーのエントリーポイント
start:
    cli                 ; 割り込み無効化
    xor ax, ax          ; AXレジスタをクリア
    mov ds, ax          ; データセグメントを0に設定
    mov es, ax          ; エクストラセグメントを0に設定
    mov ss, ax          ; スタックセグメントを0に設定
    mov sp, 0x7C00      ; スタックポインタを設定
    sti                 ; 割り込み有効化

    ; 画面をクリア
    mov ah, 0x00        ; ビデオモード設定
    mov al, 0x03        ; テキストモード 80x25
    int 0x10            ; BIOSビデオサービス呼び出し

    ; 起動メッセージを表示
    mov si, boot_msg    ; メッセージのアドレスをSIに設定
    call print_string   ; 文字列表示関数を呼び出し

    ; カーネルをロード
    mov si, load_msg    ; カーネルロードメッセージ
    call print_string   ; 表示
    call load_kernel    ; カーネルをロード

    ; カーネルに制御を移す
    mov si, jump_msg    ; ジャンプメッセージ
    call print_string   ; 表示
    jmp 0x1000:0x0000   ; カーネルにジャンプ

; 文字列を表示する関数
print_string:
    mov ah, 0x0E        ; テレタイプ出力
    mov bh, 0x00        ; ページ番号
    mov bl, 0x07        ; 文字色（白）

.loop:
    lodsb               ; SIから1バイト読み込みALに格納し、SIをインクリメント
    or al, al           ; ALが0（文字列の終端）かチェック
    jz .done            ; 0なら終了
    int 0x10            ; 文字を表示
    jmp .loop           ; 次の文字へ

.done:
    ret                 ; 呼び出し元に戻る

; カーネルをロードする関数
load_kernel:
    mov ah, 0x02        ; ディスク読み込み関数
    mov al, 10          ; 読み込むセクタ数（カーネルのサイズに応じて調整）
    mov ch, 0           ; シリンダ番号
    mov cl, 2           ; セクタ番号（1はブートセクタ、2から始まる）
    mov dh, 0           ; ヘッド番号
    mov dl, 0x80        ; ドライブ番号（ハードディスク）
    mov bx, 0x1000      ; ESセグメント
    mov es, bx
    mov bx, 0           ; オフセット
    int 0x13            ; BIOSディスクサービス呼び出し
    jc .error           ; エラーが発生した場合

    ret                 ; 正常終了

.error:
    mov si, error_msg   ; エラーメッセージ
    call print_string   ; 表示
    jmp $               ; 無限ループ

; メッセージ
boot_msg db 'Ultra Fast OS Bootloader starting...', 13, 10, 0
load_msg db 'Loading kernel into memory...', 13, 10, 0
jump_msg db 'Jumping to kernel...', 13, 10, 0
error_msg db 'Error loading kernel!', 13, 10, 0

; ブートセクタの終了マーク
times 510-($-$$) db 0   ; 残りのスペースを0で埋める
dw 0xAA55               ; ブートシグネチャ