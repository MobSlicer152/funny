	BITS 16
	ORG 7c00h

	; just in case loaded at 7c00:0000 instead of 0000:7c00
	jmp 0:Entry

; Runs at 7c00h
Entry:
	; data 7E00h-17E00h
	xor ax, ax
	mov ds, ax
	mov es, ax
	; stack 17E00h-1FE00h
	mov ax, 17e0h
	mov ss, ax
	mov sp, 8000h

	; read partition 1
	mov ah, 2 ; read sectors
	mov al, cs:[kernelSize] ; however many sectors the partition is
	mov ch, 0 ; 0th cylinder
	mov cl, 2 ; 2nd sector
	mov dh, 0 ; 0th head
	mov bx, 7e00h ; ES:BX is 0000:7E00
	int 13h

	; check if the read failed
	jc .exit

	; check if enough sectors were read
	cmp al, cs:[kernelSize]
	jne .exit

	; enter the main binary at 7e00h
	; load CS with 0000h
	jmp 0:7e00h

	jmp .exit

.exit:
	; print an error
	mov si, errorMsg
	call PrintStr

	; need hlt to work
	cli
.end:
	hlt
	jmp .end

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

errorMsg:
	DB "Failed to read kernel partition", 0dh, 0ah, 00h

%include "mbr.inc"

