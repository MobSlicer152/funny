	; padding
	TIMES 440 - ($ - $$) DB 0

	; MBR partition table
	DD 69420694h ; unique ID
	DW 0 ; reserved

	; partition 1
	DB 80h ; active
	; start offset in CHS
	DB 0 ; head 0
	DB 00001000b ; sector 2
	DB 0 ; cylinder 0
	DB 0c2h ; hidden Linux
	; end offset in CHS
	DB 0 ; head 0
	DB 01100100b ; sector 49
	DB 0 ; cylinder 0
	DD 1 ; starting LBA offset
	DD 30h ; total sectors (48 sectors = 24KB)

	; partitions 2-4 are unused
	TIMES 3 * 16 DB 0

	; MBR signature
	DB 55h, 0aah
