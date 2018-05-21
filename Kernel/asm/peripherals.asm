GLOBAL getTimeFromRTC
GLOBAL readFromKeyboard
GLOBAL picMasterMask
GLOBAL picSlaveMask

; The following function gets the segment of time that is specified by this function's only parameter from the RTC
; where the available values are: 0 for seconds, 2 for minutes, 4 for hours, 6 for day of week, 7 for day of month
; 8 for month and 9 for year.

getTimeFromRTC:
	
	mov rax, rdi
	out 70h, al
	in al, 71h
	ret

; As the name implies it reads a single key from the keyboard. If there isn't anything to be read, returns -1

readFromKeyboard:

	in al, 64h
	test al, 1
	jz exit

	in al, 60h

	ret
	
	exit:

		mov al, -1
		ret

; sets the IMR to the 8 bit number given by this function's first parameter thus enabling/disabling the desired
; hardware exceptions.

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

; sets the slave PIC's IMR to the 8 bit number given by this function's first parameter thus enabling/disabling
; the desired hardware exceptions.

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn