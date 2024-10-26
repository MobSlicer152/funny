ENTRY(0x0)

SECTIONS
{
	.startup 0x0 : {
		*(.startup)
	}
	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}

