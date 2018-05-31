GLOBAL test

section .text

test:

	mov eax, 1
	mov rdi, 1
	mov rsi, cadena
	mov rdx, 10

	int 80h

	mov eax, 2
	mov rdi, 1
	mov rsi, cadena
	mov rdx, 10

	int 80h

	ret

section .data
cadena db "hola mundo todo piola"