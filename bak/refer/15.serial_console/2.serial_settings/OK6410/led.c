#define GPKCON (volatile unsigned long*)0x7f008800
#define GPKDAT (volatile unsigned long*)0x7f008808

void led_init()
{
    *(GPKCON) = 0x1111;
}

void led_on()
{
    *(GPKDAT) = 0x0e;
}
    
void led_off()
{
    *(GPKDAT) = 0x0f;
}