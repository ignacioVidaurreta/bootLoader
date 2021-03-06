GLOBAL mutex_lock

section .text

mutex_lock:
	push rbx
	push rcx
	push rdx

	mov rax, 0			; edx:eax = UNLOCKED = 0
	mov rbx, 1			; ecx:ebx = LOCKED	= 1
	mov rcx, 0
	mov rdx, 0

	cmpxchg8b [rdi]		; si edx:eax == [rdi] ==> set [rdi] a ecx:ebx y setear el flag ZF

	jz was_unlocked		;jump to was_unlocked si ZF fue seteado

was_locked: 			;Si llego acá entonces zf no fue seteado ==> estaba locked
	mov rax, 1			; LOCKED = 1, return wasLocked = TRUE
	pop rdx
	pop rcx
	pop rbx
	ret

was_unlocked:
	mov rax, 0		; UNLOCKED	= 0, return wasLocked = FALSE
	pop rdx
	pop rcx
	pop rbx
	ret
