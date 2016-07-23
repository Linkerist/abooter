
#include <sunshine/led.h>

void led_init()
{   
    *(GPBCON) = GPB5_out | GPB6_out | GPB7_out | GPB8_out;
}

void led_off()
{
    *(GPBDAT) = 0x7ff;
}

void led_on()
{
    *(GPBDAT) = 0x61f;
}

