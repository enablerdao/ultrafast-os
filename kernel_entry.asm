; カーネルエントリーポイント
[BITS 32]       ; 32ビットモードで実行
global _start   ; エントリーポイントをグローバルに公開

; マルチブートヘッダー
section .multiboot
align 4
    dd 0x1BADB002            ; マジックナンバー
    dd 0x00                  ; フラグ
    dd -(0x1BADB002 + 0x00)  ; チェックサム

; カーネルのエントリーポイント
section .text
_start:
    ; スタックを設定
    mov esp, stack_top

    ; カーネルのメイン関数を呼び出す
    extern kernel_main
    call kernel_main

    ; 無限ループ（カーネルが戻ってきた場合）
    cli                     ; 割り込み無効化
.hang:
    hlt                     ; CPUを停止
    jmp .hang               ; 無限ループ

; スタックセクション
section .bss
align 16
stack_bottom:
    resb 16384             ; 16KBのスタック
stack_top: