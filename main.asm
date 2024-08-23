BITS 16
ORG 0000h

STRUC GDT_DESCRIPTOR
	.size: resw 1
	.offset: resd 1
ENDSTRUC

STRUC GDT_SEGMENT_DESCRIPTOR
	
ENDSTRUC

	jmp Main

	SECTION .code
Main:
	; set mode 0
	mov ah, 0
	mov al, 0
	int 10h

	; set cursor to page 0 at (0, 0)
	mov ah, 2
	mov bh, 0
	xor dx, dx
	int 10h

	; print message
	mov si, startMsg
	call PrintStr

	; check A20
	call CheckA20
	cmp al, 1
	je .A20Enabled

	; enable A20
	call EnableA20

.A20Enabled:
	mov si, A20EnabledMsg
	call PrintStr

	

.exit:
	hlt
	jmp .exit

; print C string in DS:SI
PrintStr:
.printLoop:
	mov ah, 0eh ; teletype
	mov bx, 000fh ; page 0, white
	lodsb ; DS:SI -> AL, inc SI
	cmp al, 0 ; check for NUL
	je .printEnd
	int 10h
	jmp .printLoop
.printEnd:
	ret

; check A20 line, return in AL
CheckA20:
	mov si, checkA20Msg
	call PrintStr

	push es
	push ds

	xor ax, ax
	mov es, ax
	not ax
	mov ds, ax

	; read boot signature at 0000:7dfe
	mov cx, es:[7dfeh]
	
	; read other at ffff:7e0e
	mov dx, ds:[7e0eh]

	cmp ax, dx
	jne .secondCheck
	mov al, 0 ; return false
	jmp .return

	; check if changing first one changes other one
.secondCheck:
	rol cx, 8
	mov es:[7dfeh], cx
	mov ax, ds:[7e0eh]
	cmp ax, cx
	jne .enabled
	mov al, 0
	jmp .return

.enabled:
	mov al, 1

.return:
	pop ds
	pop es
	ret

; enable A20 line, using Fast A20
EnableA20:
	in al, 92h
	test al, 2
	jnz .done
	or al, 2
	and al, 0feh
	out 92h, al
.done:
	ret

	SECTION .data
startMsg:
	DB 'Doing a funny', 0dh, 0ah, 0
checkA20Msg:
	DB 'Checking A20 line', 0dh, 0ah, 0
enableA20Msg:
	DB 'Enabling A20 line', 0dh, 0ah, 0
A20EnabledMsg:
	DB 'A20 line enabled', 0dh, 0ah, 0
gdtLoadMsg:
	DB 'Loading GDT', 0dh, 0ah, 0

TIMES 6000h - ($ - $$) DB 0

