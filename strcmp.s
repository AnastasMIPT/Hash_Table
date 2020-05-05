.intel_syntax noprefix
.text
.globl my_strcmp

my_strcmp:
        push rbp
        mov rbp, rsp
        xor eax, eax
        push rdx
        xor rdx, rdx

        
        
.loop:
        mov dh, byte [rdi - 1]
        mov dl, byte [rsi - 1]

        cmp dh, 0
        je .Exit
        cmp dl, 0
        je .Exit

        mov al, dh
        sub al, dl
        inc rsi
        inc rdi
        cmp al, 0
        je .loop

        dec rsi
        dec rdi

.Exit:
        mov al, byte [rdi - 1]
        sub al, byte [rsi - 1]   
        pop rdx
        pop rbp
        ret
