%include "words.inc"
%define BUFFER_SIZE 255


global _start
extern read_word
extern print_string
extern print_newline
extern string_length
extern exit
extern find_word
extern write_string

section .data
input_msg: db 'Input your key >>> ',0
err_msg: db 'Not_found_entry',0

section .text
_start:
	mov rdi, input_msg
        call print_string	

	sub rsp, BUFFER_SIZE
	mov rdi, rsp
	mov rsi, BUFFER_SIZE
	call read_word
	
	mov rsi, last
	mov rdi, rax
	call find_word
	
	add rsp, BUFFER_SIZE

	test rax, rax
	jz .fail

        mov rdi, rax
        call string_length

        add rdi, rax
        inc rdi
        call print_string
        call print_newline

        mov rdi, 0
        call exit

.fail:
	mov rsi, 2
	mov rdi, err_msg
	call write_string
	call print_newline
	call exit
	
