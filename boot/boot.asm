	BITS 16
	ORG 7c00h

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

	; get the drive's CHS stuff
	call GetDriveGeometry

	; read the kernel to 1000h:0000h
	mov cx, kernelSize
	mov ax, 1000h
	mov es, ax
	xor di, di
	call Read

	; check if the read failed
	jc .exit

	; check if enough sectors were read
	cmp al, kernelSize
	jl .exit

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
	jmp 10000h
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

; get drive geometry of drive DL
GetDriveGeometry:
	push cx
	push dx

	; get drive info
	push es
	push di
	mov ah, 8
	xor di, di
	mov es, di
	int 13h
	pop di
	pop es

	; get CHS stuff (AX is SPT, BX is HPC, CX is cylinders)
	mov ax, cx
	and ax, 3fh ; bits 0-5 is sectors per track
	mov bx, cx
	shl bx, 2 ; bits 6-15 is cylinders - 1
	mov cl, ch
	mov ch, bh
	inc cx
	mov bx, dx ; DH is heads - 1
	and bx, 0ffh
	inc bx
	
	mov [driveSectorsPerTrack], ax
	mov [driveHeadsPerCylinder], bx
	mov [driveCylinders], cx

	pop dx
	pop cx

	ret

; read CX sectors starting at SI in LBA to ES:DI from drive DL
Read:
	push cx
	push dx

	; calculate CHS of LBA in SI, stack becomes
	; => sector
	;    head
	;    cylinder
	;    DX with drive index
	; ==

	mov ax, [driveSectorsPerTrack]
	mov bx, [driveHeadsPerCylinder]
	mov cx, [driveCylinders]

	; DX = cylinder = LBA / (HPC * SPT)
	push ax
	mul bl ; AX = HPC * SPT
	mov dx, ax
	mov ax, si
	div dx ; LBA / DX
	mov dx, ax
	pop ax
	push dx ; cylinder

	; DX = head = (LBA / SPT) % HPC
	push ax
	mov dx, ax
	mov ax, si
	div dx ; AX = LBA / SPT
	div bx ; DX = AX % HPC
	pop ax
	push dx ; head

	; DX = sector = (LBA % SPT) + 1
	push ax
	mov ax, si
	div cx
	inc dx
	pop ax
	push dx ; sector

	; take stuff off stack
	pop ax ; sector
	pop bx ; head
	pop cx ; cylinder

	; mutilate CX for int 13h
	mov dx, cx
	and dx, 0ffh
	shl dx, 8
	and cx, 300h
	shr cx, 2
	or cx, dx
	or cx, ax

	; do the int 13h
	mov ax, cx
	mov ah, 2
	mov 

.done:
	ret

%include "gdt.inc"

%define QUOTE "
%define STRINGIZE(x) QUOTE %+ x %+ QUOTE

errorMsg:
	DB "Failed to read kernel partition (", STRINGIZE(kernelSize), " sectors)", 0dh, 0ah, 00h

driveCylinders:
	RESW 1
driveHeadsPerCylinder:
	RESW 1
driveSectorsPerTrack:
	RESW 1

%include "mbr.inc"