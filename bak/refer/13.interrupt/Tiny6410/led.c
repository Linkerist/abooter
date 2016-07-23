#define GPKCON (volatile unsigned long*)0x7f008800
#define GPKDAT (volatile unsigned long*)0x7f008808

void led_init()
{
    *(GPKCON) = 0x11110000;
}

void led_on()
{
    *(GPKDAT) = 0x0;
}
    
void led_off()
{
    *(GPKDAT) = 0xf0;
}