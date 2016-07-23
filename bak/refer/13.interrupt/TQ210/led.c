#define GPC0CON *((volatile unsigned long*)0xE0200060)
#define GPC0DAT *((volatile unsigned long*)0xE0200064)

void led_init()
{   
    GPC0CON = 0x00011000;
}

void led_off()
{
    GPC0DAT = 0x0;
}

void led_on()
{
    GPC0DAT = 0x00000018;
}
