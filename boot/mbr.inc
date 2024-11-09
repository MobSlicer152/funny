	; padding
	TIMES 440 - ($ - $$) DB 0

	; MBR partition table
	DD 4e42534ch ; unique ID
	DW 0 ; reserved

	; partition 1
	DB 80h ; active
	; start offset in CHS
	DB 0 ; head 0
	DB 00001000b ; sector 2
	DB 0 ; cylinder 0
	DB 0c2h ; hidden Linux
	; end offset in CHS
	DB 3 ; head 3
	DB 00110100b ; sector 13
	DB 0 ; cylinder 0
	DD 1 ; starting LBA offset
	DD kernelSize

	; partitions 2-4 are unused
	TIMES 3 * 16 DB 0

	; MBR signature
	DB 55h, 0aah
