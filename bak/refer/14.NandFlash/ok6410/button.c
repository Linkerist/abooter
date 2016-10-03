#define GPNCON (volatile unsigned long*)0x7f008830

void button_init()
{
	*(GPNCON) = 0b10 | (0b10<<10);        //设置按键1与按键6
}