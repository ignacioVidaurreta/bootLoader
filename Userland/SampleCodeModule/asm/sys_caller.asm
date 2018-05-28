GLOBAL _int80

section .text

	_int80:
		push rbp
		mov rbp, rsp

		mov rax, r9
		int 80h

		mov rsp, rbp
		pop rbp
		ret