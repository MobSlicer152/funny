BITS 16
ORG 7c00h

; Runs at 7c00h
Entry:
	; zero segments, except DS
	xor ax, ax
	mov es, ax
	mov ss, ax

	mov ax, 50h
	mov ds, ax

	; data 500h-6700h
	mov ax, 50h
	mov ds, ax
	mov es, ax
	; stack 6700h-7100h
	mov ax, 670h
	mov ss, ax

	; read partition 1
	mov ah, 2 ; read sectors
	mov al, 30h ; 30 sectors
	mov ch, 0 ; 0th cylinder
	mov cl, 2 ; 2nd sector
	mov dh, 0 ; 0th head
	xor bx, bx ; ES:BX is 0500:0000
	int 13h

	; check if the read failed
	jc .exit
	cmp al, 30h
	jne .exit

	; enter the main binary at 0500:0200
	jmp 0500h:0200h

	jmp .exit

	; Dead loop if something goes wrong
.exit:
	cli ; Don't want this loop interrupted
.end:
	hlt
	jmp .end

	; padding
	TIMES 440 - ($ - $$) DB 0

	; MBR partition table
	DD 69420694h ; unique ID
	DW 0 ; reserved

	; partition 1
	DB 80h ; active
	; start offset in CHS
	DB 0 ; head 0
	DB 00001000b ; sector 2 (start from sector 1 since sector numbering starts from 1)
	DB 0 ; cylinder 0
	DB 0c2h ; hidden Linux
	; end offset in CHS (estimated to be cylinder 0, head 0, sector 49)
	DB 0 ; head 0
	DB 01100100b ; sector 49 (end at sector 48)
	DB 0 ; cylinder 0
	DD 1 ; starting LBA offset
	DD 30h ; total sectors (48 sectors = 24KB)

	; partitions 2-4 are unused
	TIMES 3 * 16 DB 0

	; MBR signature
	DB 55h, 0aah

