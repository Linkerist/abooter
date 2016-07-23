
#include <asm/mach/interrupts.h>

#include <sunshine/led.h>
#include <net/dm9000.h>

#include <adc/ts.h>

void init_irq()
{

    // 对于EINT4，需要在EINTMASK寄存器中使能它
    EINTMASK &= ~(1 << 4);
            

    // EINT0、EINT1、EINT2、EINT4_7使能
    INTMSK   &= (~(1 << 0)) & (~(1 << 1)) & (~(1 << 2)) & (~(1 << 4));
    
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
	unsigned long value = INTOFFSET;
	
	switch(value){
		case 0: //EINT0~K4
			led_on();
		    break;
		
		case 1: //EINT1~K1
			led_off();
		    break;
		
		case 2: //EINT2~K3
		    led_on();
		    break;
		
		case 4: //EINT4~K2
		    int_issue();
		    break;
		
		case 31:
		    tc_handler(); 
		    break;
		    
		default:
		    break;
	}
	
	/* clear interrupts */
	if(value == 4)
		EINTPEND = (1 << 4);
	SRCPND = 1 << value;
	INTPND = 1 << value;
}

