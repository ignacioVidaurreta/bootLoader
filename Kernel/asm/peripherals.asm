GLOBAL getTimeFromRTC
GLOBAL readFromKeyboard
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL beepASM
GLOBAL noBeepASM

RTCConfig:
	cli
	mov al,0Bh
	out 70h,al
	in al,71h
	or al, 0100b
	out 71h,al
	sti
	ret

; The following function gets the segment of time that is specified by this function's only parameter from the RTC
; where the available values are: 0 for seconds, 2 for minutes, 4 for hours, 6 for day of week, 7 for day of month
; 8 for month and 9 for year.

getTimeFromRTC:
	call RTCConfig
	mov rax, rdi
	out 70h, al
	in al, 71h
	ret

; As the name implies it reads a single key from the keyboard. If there isn't anything to be read, returns -1

readFromKeyboard:

	in al, 64h
	test al, 1
	jz exit

	mov rax, 0
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

 beepASM:

	mov al, 0xB6		;sets the pit to channel 2 which is the PC speaker, as well as setting the square
									;wave generator and access: lobyte/highbyte
									;if I'm being honest with you I have no idea why those last two are done but it doesn't
									;work without them.
	out 43h,al
	mov rax, rdi		;frequency passed on as parameter.
	out 42h,al
	mov al,ah
	out 42h,al
	in al, 61h
	or al, 03h
	out 61h,al
  ret

 noBeepASM:

  in al, 61h
	and al, 0xFC
	out 61h,al
  ret
