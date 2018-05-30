GLOBAL btest1
GLOBAL btest2
section .text

btest1:

	mov rax, 6
	mov rdi, 1000

	int 80h

	ret

btest2:

	mov rax, 7

	int 80h

	ret