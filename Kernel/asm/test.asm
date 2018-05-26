GLOBAL test
GLOBAL test2
section .text

test:

	mov eax, 7
	mov rdi, 1000

	int 80h

	ret

section .data
cadena db 50