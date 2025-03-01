	BITS 16
	ORG 7c00h

	%define kernelHigh 000h
	%define kernelLow 7e00h
	; total sectors in the kernel (7fh/127 sectors = 63.5kB)
	%define kernelSize 7fh

	; just in case loaded at 7c00:0000 instead of 0000:7c00
	jmp 0:Entry

; Runs at 0000:7c00
Entry:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ax, 2000h
	mov ss, ax
	mov sp, 4000h

	; try three times in case of disk controller being shit
	xor si, si
	jmp .read
.readLoop:
	cmp si, 3
	jae .exit
	inc si
.read:
	; TODO: 18 sectors at a time for floppy to work

	; read partition 1
	mov ah, 2 ; read sectors
	mov al, kernelSize ; however many sectors the partition is
	mov ch, 0 ; 0th cylinder
	mov cl, 2 ; 2nd sector
	mov dh, 0 ; 0th head
	; DL is already the drive this booted from, where the kernel should be
	; ES:BX is destination
	mov bx, kernelHigh
	mov es, bx
	mov bx, kernelLow
	int 13h

	; check if the read failed
	jc .readLoop

	; check if enough sectors were read
	cmp al, kernelSize
	jl .readLoop

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
	or eax, 1 ; bit 0 is
	mov cr0, eax

	; set CS to GDT code descriptor
	jmp 08h:.enterKernel

	; need 32-bit for > 0ffffh jump
	BITS 32
.enterKernel:
	jmp 7e00h
	BITS 16

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