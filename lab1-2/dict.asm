global find_word
extern string_equals

section .text


;rdi - указатель на ключ
;rsi - указатель на лист
find_word:
  xor rax, rax
  .index:
    test rsi, rsi
    jz .end
    
    add rsi, 8
    call string_equals

    test rax, rax
    jnz .end 

    mov rsi, [rsi-8]
    jmp .index 	
  .end:
    mov rax, rsi
    ret 	
