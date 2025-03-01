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

	; new ESP, has to match STACK_END in consts/memmap.h
	mov esp, 1be00h

	; enter C code (no call cause it doesn't return)
	jmp KernelMain
