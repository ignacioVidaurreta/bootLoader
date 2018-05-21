section .text
GLOBAL test

test:
	mov rsi, cadena 	; Puntero a la cadena
	mov rdx, 12			; Largo de la cadena 
	mov rdi, 2			; FileDescriptor (STDOUT)
	mov rax, 4			; ID del Syscall WRITE
	int 80h				; Ejecucion de la llamada

	ret

section .data
cadena db "Hola Mundo!!"