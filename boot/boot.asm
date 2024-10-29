	BITS 16
	ORG 7c00h

	; total sectors (80h/128 sectors = 65.5kB)
	%define kernelSize 080h

	; just in case loaded at 7c00:0000 instead of 0000:7c00
	jmp 0:Entry

; Runs at 7c00h
Entry:
	; kernel 7e00h-17e00
	xor ax, ax
	mov ds, ax
	mov es, ax
	; stack 100000h-108000h
	mov ax, 1000h
	mov ss, ax
	mov sp, 8000h

	; read partition 1
	mov ah, 2 ; read sectors
	mov al, kernelSize ; however many sectors the partition is
	mov ch, 0 ; 0th cylinder
	mov cl, 2 ; 2nd sector
	mov dh, 0 ; 0th head
	; dl is already the drive this booted from, where the kernel should be
	mov bx, 7e00h ; ES:BX is 0000:7e00
	int 13h

	; check if the read failed
	jc .exit

	; check if enough sectors were read
	cmp al, kernelSize
	jne .exit

	; enter the main binary at 7e00h
	jmp 0:7e00h

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

%define QUOTE
%define STRINGIZE(x) QUOTE %+ x %+ QUOTE

errorMsg:
	DB "Failed to read kernel partition (", STRINGIZE(kernelSize), ")", 0dh, 0ah, 00h

%include "mbr.inc"

