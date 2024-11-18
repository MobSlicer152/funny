	BITS 16
	ORG 7c00h

	; total sectors in the kernel (80h/128 sectors = 65.5kB)
	%define kernelSize 80h

	; just in case loaded at 7c00h:0000h instead of 0000h:7c00h
	jmp 0:Entry

; Runs at 0000h:7c00h
Entry:
	; kernel 7e00h-17e00h
	xor ax, ax
	mov ds, ax
	mov es, ax
	; stack 17e00h-1be00h, backbuffer 1be00h-2b800, interrupt stack 2b800h-2c800h
	mov ax, 17eh
	mov ss, ax
	mov sp, 4000h

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

	; change video mode to 320x200 256-colour
	mov ah, 0
	mov al, 13h
	int 10h

	; check A20
	call CheckA20
	cmp al, 1
	je .A20Enabled

	; enable A20
	call EnableA20

.A20Enabled:
	; load the GDT
	mov eax, gdt
	mov ds:[gdtr + GDT_DESCRIPTOR.offset], eax
	mov eax, gdtEnd
	sub eax, gdt
	dec eax ; subtract one
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

	; Enter the 32-bit kernel
	jmp 08h:7e00h

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
	ret

; check A20 line, return in AL
CheckA20:
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

%include "gdt.inc"

%define QUOTE "
%define STRINGIZE(x) QUOTE %+ x %+ QUOTE

errorMsg:
	DB "Failed to read kernel partition (", STRINGIZE(kernelSize), " sectors)", 0dh, 0ah, 00h

%include "mbr.inc"