; Real mode non-boot sector startup code, sets video mode with BIOS calls,
; checks and enables the A20 line, sets up a GDT, and enters protected mode.

	BITS 16

	EXTERN KernelMain

	SECTION .startup
Main:
	; set mode 2
	;mov ah, 0
	;mov al, 2
	;int 10h

	; set cursor to page 0 at (0, 0)
	;mov ah, 2
	;mov bh, 0
	;xor dx, dx
	;int 10h

	; change video mode to 320x200 256-colour
	mov ah, 0
	mov al, 13h
	int 10h

	; print message
	;mov si, startMsg
	;call PrintStr

	; check A20
	call CheckA20
	cmp al, 1
	je .A20Enabled

	; enable A20
	call EnableA20

.A20Enabled:
	;mov si, A20EnabledMsg
	;call PrintStr

	; load the GDT
	;mov si, protectedModeMsg
	;call PrintStr

	xor eax, eax
	add eax, gdt
	mov ds:[gdtr + GDT_DESCRIPTOR.offset], eax
	mov eax, gdtEnd
	sub eax, gdt
	mov ds:[gdtr + GDT_DESCRIPTOR.size], ax

	; no interrupts during this
	cli

	jmp .flush
.flush:
	lgdt ds:[gdtr]

	; enable protected mode
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp 08h:.protectedMode ; far jump into protected mode code to set CS to a GDT selector

.protectedMode:
	BITS 32

	; load other segments with GDT selector for "data" segment
	mov ax, 10h
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	; SS is now mapped to 00000h, have to adjust ESP
	add esp, 17e00h

	call KernelMain

	BITS 16

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
	;mov si, checkA20Msg
	;call PrintStr

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
	;mov si, enableA20Msg
	;call PrintStr

	in al, 92h
	test al, 2
	jnz .done
	or al, 2
	and al, 0feh
	out 92h, al
.done:
	ret

startMsg:
	DB 'Doing a funny', 0dh, 0ah, 0
checkA20Msg:
	DB 'Checking A20 line', 0dh, 0ah, 0
enableA20Msg:
	DB 'Enabling A20 line', 0dh, 0ah, 0
A20EnabledMsg:
	DB 'A20 line enabled', 0dh, 0ah, 0
protectedModeMsg:
	DB 'Loading GDT and entering protected mode', 0dh, 0ah, 0

%include "gdt.inc"

