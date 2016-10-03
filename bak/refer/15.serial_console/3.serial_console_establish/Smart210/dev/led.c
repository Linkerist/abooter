#define GPC0CON *((volatile unsigned long*)0xE0200280)
#define GPC0DAT *((volatile unsigned long*)0xE0200284)

void led_init()
{   
    GPC0CON = 0x1;
}

void led_off()
{
    GPC0DAT = 0x1;
}

void led_on()
{
    GPC0DAT = 0x0;
}
