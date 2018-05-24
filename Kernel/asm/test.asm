GLOBAL test
section .text

extern ncPrintDec
extern ncPrintUser

test:

	mov eax, 1
	mov rsi, cadena
	mov rdi, 1
	mov rdx, 10

	int 80h

	mov rdx, 10
	mov rax, 2
	mov rdi, 3
	mov rsi, cadena

	int 80h
	ret

section .data
cadena db 50