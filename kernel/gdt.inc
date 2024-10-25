STRUC GDT_DESCRIPTOR
	.size: RESW 1
	.offset: RESD 1
ENDSTRUC

STRUC GDT_SEGMENT_DESCRIPTOR
	.limit: RESW 1
	.base: RESB 3
	.access: RESB 1
	.limitEx: ; these share a byte
	.flags: RESB 1
	.baseEx: RESB 1
ENDSTRUC

gdtr:
	RESB GDT_DESCRIPTOR_size

gdt:
	; null descriptor
	ISTRUC GDT_SEGMENT_DESCRIPTOR
		AT .limit, DW 0
		AT .base, DB 0, 0, 0
		AT .access, DB 0
		AT .limitEx, DB 0
		AT .baseEx, DB 0
	IEND
	; code descriptor
	ISTRUC GDT_SEGMENT_DESCRIPTOR
		AT .limit, DW 0ffffh
		AT .base, DB 00h, 00h, 00h
		AT .access, DB 0b10011011
		AT .limitEx, DB 0cfh
		AT .baseEx, DB 00h
	IEND
	; data descriptor
	ISTRUC GDT_SEGMENT_DESCRIPTOR
		AT .limit, DW 0ffffh
		AT .base, DB 00h, 00h, 00h
		AT .access, DB 0b10010011
		AT .limitEx, DB 0cfh
		AT .baseEx, DB 00h
	IEND
gdtEnd:

