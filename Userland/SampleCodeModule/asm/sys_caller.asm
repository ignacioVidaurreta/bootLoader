GLOBAL int80

section .text

	int80:
		push rbp
		mov rbp, rsp

		mov rax, r9
		int 80h

		mov rsp, rbp
		pop rbp
		ret
