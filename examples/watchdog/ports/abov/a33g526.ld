MEMORY
{
	rom (rx)	: ORIGIN = 0x00000000, LENGTH = 256K
	rom_data (rw)	: ORIGIN = 0x0F000000, LENGTH = 32K
	ram (rwx)	: ORIGIN = 0x20000000, LENGTH = 24K
}

INCLUDE armv7-m-generic.ld
