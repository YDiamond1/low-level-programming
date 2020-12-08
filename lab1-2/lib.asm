
section .text

global string_length
global print_string
global print_newline
global read_word 
global string_equals
global write_string
global exit 
; Принимает код возврата и завершает текущий проц
exit:
    mov rax, 60
    syscall
    ret

; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
    xor rax, rax
    .loop:
      cmp byte[rdi+rax], 0; сравниваем нуль с окончанием строки (нуль-терминант)
      je .exit; проверяем условие, если равны, то возвращаем значение
      inc rax; иначе инкрементим аккумулятор
      jmp .loop; и возвращаемся к метке
    .exit:
      ret

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
print_string:
    call string_length; узнаем длину строки
    mov rsi, rdi; передаем строку для вывода
    mov rdi, 1; передаем дескриптор на stdout
    mov rdx, rax; передаем длину строки полученную с вызова string_length
    mov rax, 1; код write
    syscall
    ret


write_string:
        push r11
	call string_length
	mov r11, rsi
	mov rsi, rdi; передаем строку для вывода
        mov rdi, r11; передаем дескриптор на stdout
        mov rdx, rax; передаем длину строки полученную с вызова string_length
        mov rax, 1; код write
        syscall
	pop r11	
        ret 


; Принимает код символа и выводит его в stdout
print_char:
    push rdi
    mov rsi, rsp; передаем символ для вывода
    mov rdi, 1; передаем дескриптор на stdout
    mov rdx, 1; передаем длину строки полученную с вызова string_length
    mov rax, 1; код write
    syscall
    pop rdi
    ret

; Переводит строку (выводит символ с кодом 0xA)
print_newline:
    mov rdi, 10
    call print_char
    ret
    

; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
   push rbp
   mov rbp, 1
   mov rax, rdi
   mov rdi, 10 
   dec rsp
   mov byte[rsp], 0
   .loop:
   	dec rsp
	inc rbp
   	xor rdx, rdx
	div rdi
	add rdx, '0'
	mov byte[rsp], dl
	test rax, rax
	jnz .loop 
   mov rdi, rsp
   call print_string
   add rsp, rbp
   pop rbp
   ret

; Выводит знаковое 8-байтовое число в десятичном формате 
print_int:
    test rdi, rdi
    jns .unsigned
    push rdi
    mov rdi, '-'
    call print_char
    pop rdi
    neg rdi
    .unsigned:
    	jmp print_uint

; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
    push r11
    push rdi
    push rsi
    call string_length
    mov rcx, rax
    mov r11, rdi
    mov rdi, rsi 
    call string_length
    cmp rax, rcx
    jne .not_equals
    xor rdx, rdx
    .loop:
    	xor rax, rax
    	mov al, byte[rdi+rdx]
    	cmp byte[r11+rdx], al
	jne .not_equals
	cmp rdx, rcx
	jne .inc
	mov rax, 1
	pop rsi
 	pop rdi
	pop r11
	ret
    .inc:
	inc rdx
	jmp .loop
    .not_equals:
	pop rsi
	pop rdi
        pop r11
    	mov rax, 0
    	ret

; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
    push rdi
    push rsi
    dec rsp
    mov rax, 0
    mov rdi, 0
    mov rsi, rsp
    mov rdx, 1
    syscall

    test rax, rax
    jnz .end
    mov byte[rsp], 0x0

    .end:
    	mov al, byte[rsp]
    	inc rsp
	pop rsi
	pop rdi
    	ret 

; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0x10.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера. 
; Эта функция должна дописывать к слову нуль-терминатор

read_word:
    xor rcx, rcx
.loop:
    cmp rcx, rsi
    jg .fail
    
    push rdx
    push rcx
    push rsi
    push rdi
    mov rdi, rdx
    call read_char
    pop rdi
    pop rsi
    pop rcx
    pop rdx

    cmp al, 0x20
    je .success
    cmp al, 0xA
    je .success
    cmp al, 0x9
    je .success
    cmp al, 0       ; EOF
    jle .end

    mov byte[rdi + rcx], al
    inc rcx
    jmp .loop

.fail:
    mov rax, 0
    ret

.success:
    test rcx, rcx
    jz .loop
.end:
    mov byte[rdi + rcx], 0x0
    mov rax, rdi
    mov rdx, rcx
    ret



 

; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
    push r13
    push r14
    push r15
    mov r14, 10
    xor r13, r13
    xor rax, rax
    xor rdx, rdx
    xor rcx, rcx
    xor rsi, rsi
    .space:
	cmp byte[rdi+rcx], 0x9
	je .inc
	cmp byte[rdi+rcx], 0x20
	je .inc
	cmp byte[rdi+rcx], 0xA
	je .inc
	cmp byte[rdi+rcx], 0x0
	je .end

    .numbers:
    	xor r13, r13
    	cmp byte[rdi+rcx], '0'
	jb .end
	cmp byte[rdi+rcx], '9'
	ja .end

	mov r13b, byte[rdi+rcx]
	sub r13, '0'

	mul r14
	add rax, r13
	inc rcx
	inc r15
	jmp .numbers
 
    .inc:
    	inc rcx
	jmp .space
    .end:
    	mov rdx, r15
	pop r15
    	pop r14
	pop r13
    	ret




; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
    xor rax, rax
    xor rcx, rcx
    xor rdx, rdx
    push rdi
    .space:
        cmp byte[rdi+rcx], 0x9
	je .inc
	cmp byte[rdi+rcx], 0x20
	je .inc
	cmp byte[rdi+rcx], 0xA
	je .inc
	cmp byte[rdi+rcx], 0x0
	je .end
    .znak:
    	cmp byte[rdi+rcx], '-'
	jne .number
	inc rdi
	inc rcx
    .number:
	push rcx
	call parse_uint
	pop rcx
	
	test rcx, rcx
	jz .end
	neg rax
	inc rdx
	jmp .end
    .inc:
   	inc rcx
       	jmp .space
    .end:
        pop rdi    
    	ret 

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
string_copy:
    call string_length
    cmp rdx, rax 
    jle .not_disk_space
    push rdi
    push rsi
    push rax
    
    .loop:
    	xor rax, rax
	mov al, byte[rdi]
	mov byte[rsi], al
	lea rdi, [rdi+1]
	lea rsi, [rsi+1]
	cmp al, 0
	jne .loop
	pop rax
	pop rsi
	pop rdi
	ret
    .not_disk_space:
    	mov rax, 0
	ret
