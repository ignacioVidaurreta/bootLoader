GLOBAL getTimeFromRTC
GLOBAL readFromKeyboard
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL beepASM
GLOBAL noBeepASM

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
    out		0A1h,al
    pop     rbp
    retn

; puts the desired value into the specified I/O register
beepASM:
	mov al, 0xB6
	out 0x43, al 		;sets the PIT channel 2 corresponding to the beeper.
						;also sets it as a square wave generator.
	mov rax, rdi		;sets the frequency of the speaker to that which
	out 0x42, al 		;was passed as an arguement.
	mov ah, al
	out 0x42, al

	in al, 0x61
	mov al, 0x03
	out 0x61, al

	ret

; reads from the desired I/O register
noBeepASM:
	
	in al, 0x61
	mov al, 0
	out 0x61, al

	ret