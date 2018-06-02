GLOBAL invOpcodeException
GLOBAL zeroDivException

section .text

invOpcodeException:

	UD2
	ret

zeroDivException
	
	mov rax, 1
	mov rdx, 0

	div rdx