GLOBAL clearI
GLOBAL setI
GLOBAL halt
GLOBAL haltCPU

GLOBAL irq00Handler
GLOBAL irq01Handler
GLOBAL irq02Handler
GLOBAL irq03Handler
GLOBAL irq04Handler
GLOBAL irq05Handler

GLOBAL exception0Handler
GLOBAL exception6Handler

GLOBAL getIDTBaseAddress

GLOBAL int80Handler

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN int80
EXTERN main
section .text

%macro pushState 0
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rsi
	push rdi
	push rbp
	push rdx
	push rcx
	push rbx
	push rax
%endmacro

%macro popState 0
	pop rax
	pop rbx
	pop rcx
	pop rdx
	pop rbp
	pop rdi
	pop rsi
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
%endmacro

%macro pushStateWithReturn 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popStateWithReturn 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro
	
; the following two macros call the C function in charge of handling exceptions/interrupts given
; the IRQ number passed on by the following functions.

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	mov rsi, rsp
	call exceptionDispatcher

	popState

	mov qword [rsp], main
	iretq
%endmacro

; 8254 Timer (Timer Tick)
irq00Handler:
	irqHandlerMaster 0

; Keyboard
irq01Handler:
	irqHandlerMaster 1

; Cascade pic never called
irq02Handler:
	irqHandlerMaster 2

; Serial Port 2 and 4
irq03Handler:
	irqHandlerMaster 3

; Serial Port 1 and 3
irq04Handler:
	irqHandlerMaster 4

; USB
irq05Handler:
	irqHandlerMaster 5


; Zero Division Exception
exception0Handler:

	exceptionHandler 0

; Invalid Opcode exception
exception6Handler:
	exceptionHandler 1

; handles systemcalls called with the int 80h instruction
; el orden de los registros al recibir parámetros en C en
; 64 bits es rdi, rsi, rdx, r10, r8, r9 este es el mismo
; orden en el que se reciben las syscalls de linux en 64
; bits.
int80Handler:
	
	pushStateWithReturn
	mov r9, rax
	call int80
	popStateWithReturn
	iretq

; halts the CPU until an external interrupt is fired (this is simply what the hlt instruction does).

halt:
	sti
	hlt
	ret

; halts CPU forever

haltCPU:
	cli
	hlt
	jmp haltCPU

; disables hardware interruptions.

clearI:
	cli
	ret

; enables hardware interruptions.

setI:
	sti
	ret