ENTRY(0x10000)

SECTIONS
{
	.startup 0x10000 : {
		*(.startup)
	}
	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}
