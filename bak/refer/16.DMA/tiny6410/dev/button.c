#define GPNCON (volatile unsigned long*)0x7f008830
#define GPL1CON (volatile unsigned long*)0x7f008814

void button_init()
{
	*(GPNCON) = 0b10 | (0b10<<2);

	*(GPL1CON) = 0b0011<<16;
}