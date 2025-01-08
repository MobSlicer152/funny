ENTRY(0x7e00)

SECTIONS
{
	.startup 0x7e00 : {
		*(.startup)
	}
	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}
