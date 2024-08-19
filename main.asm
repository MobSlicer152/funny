BITS 16
ORG 200h

Main:
	; set mode
	xor ax, ax ; function 0, mode 0
	int 10h

	; set cursor to page 0 at (0, 0)
	mov ah, 2
	mov bh, 0
	xor dx, dx
	int 10h

	; print message
	mov si, startMsg
	call PrintStr

.exit:
	hlt
	jmp .exit

; print C string in SI
PrintStr:
	mov si, sp
	mov si, ss:[si - 8]
	mov ah, 0eh ; teletype
	mov bx, 000fh ; page 0, white
.printLoop:
	lodsb
	cmp al, 0 ; check for NUL
	je .printEnd
	int 10h
	inc si ; next char
	jmp .printLoop
.printEnd:
	ret

startMsg:
	DB 'Doing a funny', 0

TIMES 6000h - ($ - $$) DB 0
