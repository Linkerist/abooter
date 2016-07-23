
#include <asm/mach/button.h>

void button_init()
{   
        *(GPFCON) &= ~(GPF0_msk | GPF1_msk | GPF2_msk | GPF4_msk);
        *(GPFCON) |= GPF0_int | GPF1_int | GPF2_int | GPF4_int;
}

