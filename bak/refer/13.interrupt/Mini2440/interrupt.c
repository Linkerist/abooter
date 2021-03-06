/*interrupt registes*/
#define SRCPND              (volatile unsigned long *)0x4A000000
#define INTMOD              (volatile unsigned long *)0x4A000004
#define INTMSK              (volatile unsigned long *)0x4A000008
#define PRIORITY            (volatile unsigned long *)0x4A00000c
#define INTPND              (volatile unsigned long *)0x4A000010
#define INTOFFSET           (volatile unsigned long *)0x4A000014
#define SUBSRCPND           (volatile unsigned long *)0x4A000018
#define INTSUBMSK           (volatile unsigned long *)0x4A00001c

#define EINTMASK            (volatile unsigned long *)0x560000a4
#define EINTPEND            (volatile unsigned long *)0x560000a8

void init_irq()
{

    // 在EINTMASK寄存器中使能它们
    *(EINTMASK) &= (~(1<<8)) & (~(1<<11)) & (~(1<<13)) & (~(1<<14));

    // EINT8_23使能
    *(INTMSK)   &= ~(1<<5);
    
    __asm__( 
    /*开中断*/  
    "mrs r0,cpsr\n"
    "bic r0, r0, #0x80\n"
    "msr cpsr_c, r0\n"            
    : 
    : 
  );
}

void handle_int()
{
	/*读取产生中断的源*/
	unsigned long value = *(EINTPEND) & ((1<<8)|(1<<11)|(1<<13)|(1<<14));
	
	switch(value) 
	{
		case (1<<8): //K1
			led_on();
		    break;
		
		case (1<<11): //K2
			led_off();
		    break;
		
		case (1<<13): //K3
		    led_on();
		    break;
		
		case (1<<14): //K4
		    led_off();
		    break;
		
		default:
		    break;
	}
	
	/* 中断清除 */
	*(EINTPEND) = value;
	
	*(SRCPND) = 1 << 5;
	*(INTPND) = 1 << 5;
}
