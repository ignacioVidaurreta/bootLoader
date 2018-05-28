GLOBAL test

section .text

test:

	mov eax, 2
	mov rdi, 1
	mov rsi, cadena
	mov rdx, 12

	int 80h

	ret

section .data
cadena db "hello world!"