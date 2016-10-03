
#define GPBCON (volatile unsigned long*)0x56000010
#define GPBDAT (volatile unsigned long*)0x56000014

void led_init()
{   
    *(GPBCON) = 0x400;
}

void led_off()
{
    *(GPBDAT) = 0x7ff;
}

void led_on()
{
    *(GPBDAT) = 0x0;
}
