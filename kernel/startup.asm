; Loads segments and 

	BITS 32

	EXTERN KernelMain

	SECTION .startup
Main:
	; load other segments with GDT selector for "data" segment
	mov ax, 10h
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	; SS is now mapped to 00000h, have to adjust ESP
	add esp, 17e00h

	; enter C code
	call KernelMain

