	BITS 16
	ORG 7c00h

	; just in case loaded at 7c00:0000 instead of 0000:7c00
	jmp 0:Entry

; Runs at 7c00h
Entry:
	; data 500h-6500h
	xor ax, ax
	mov ds, ax
	mov es, ax
	; stack 6500h-7900h
	mov ax, 650h
	mov ss, ax
	mov sp, 1400h

	; read partition 1
	mov ah, 2 ; read sectors
	mov al, 30h ; 48 sectors
	mov ch, 0 ; 0th cylinder
	mov cl, 2 ; 2nd sector
	mov dh, 0 ; 0th head
	mov bx, 500h ; ES:BX is 0000:0500
	int 13h

	; check if the read failed
	jc .exit
	cmp al, 30h
	jne .exit

	; enter the main binary at 500h
	; load CS with 0000h
	jmp 0:500h

	jmp .exit

	; Dead loop if something goes wrong
.exit:
	cli ; Don't want this loop interrupted
.end:
	hlt
	jmp .end

%include "mbr.inc"

