#define GPNCON (volatile unsigned long*)0x7f008830

void button_init()
{
	*(GPNCON) = 0b10 | (0b10<<10);        //���ð���1�밴��6
}