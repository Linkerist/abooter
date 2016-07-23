#define GPMCON (volatile unsigned long*)0x7f008820
#define GPMDAT (volatile unsigned long*)0x7f008824

void led_init()
{
    *(GPMCON) = 0x1111;
}

void led_on()
{
    *(GPMDAT) = 0x0e;
}
    
void led_off()
{
    *(GPMDAT) = 0x0f;
}
