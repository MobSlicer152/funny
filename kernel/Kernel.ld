ENTRY(0x500)

SECTIONS
{
	.startup 0x500 : {
		*(.startup)
	}
	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}

