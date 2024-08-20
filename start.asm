BITS 16
ORG 500h

; Runs at 7c00h
Entry:
	; no interrupts
	cli

	; zero segments
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax

	; temporary location for drive index
	mov di, 700h
	mov ds:[di], dl

	; relocate to 500h
	mov si, 7c00h
	mov di, 500h
	mov cx, 200h
	rep movsb

	; jump to bootstrap code
	jmp 0:Bootstrap

; Runs at 500h
Bootstrap:
	; re-enable interrupts
	sti

	; data 500h-6700h
	mov ax, 500h
	mov ds, ax
	; stack 6700h-7100h
	mov ax, 6700h
	mov ss, ax

	; read partition 1
	mov ah, 2 ; read sectors
	mov al, 30h ; 30 sectors
	mov ch, 0 ; 0th cylinder
	mov cl, 2 ; 2nd sector
	mov dh, 0 ; 0th head
	; read drive index
	mov si, 200h
	mov dl, ds:[si]
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

