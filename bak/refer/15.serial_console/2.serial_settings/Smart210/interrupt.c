/*interrupt registes*/
#define EXT_INT_2_CON       *((volatile unsigned int *)0xE0200E08)   
#define EXT_INT_2_MASK      *((volatile unsigned int *)0xE0200F08)       
#define VIC0INTENABLE       *((volatile unsigned int *)0xF2000010)   
#define key_VICADDR         *((volatile unsigned int *)0xF2000140)     
#define VIC0ADDRESS         *((volatile unsigned int *)0xF2000F00)   
#define EXT_INT_2_PEND      *((volatile unsigned int *)0xE0200F48)  

void key_isr()
{
    volatile unsigned int key_code;
    
    __asm__( 
    
    "sub lr, lr, #4\n"  
    "stmfd sp!, {r0-r12, lr}\n"       
    : 
    : 
   );

 
    key_code = EXT_INT_2_PEND & 0b11; 
	
    switch(key_code) 
    {
        case 1: //K1
            led_on();
            break;
		
        case 2: //K2
            led_off();
            break;
		
        default:
            break;
    }
	
    /* ����ж� */    
    EXT_INT_2_PEND =  ~0x0;  
    VIC0ADDRESS = 0;
   
    __asm__( 
    "ldmfd sp!, {r0-r12, pc}^ \n"       
    : 
    : 
  );
}



void init_irq()
{
    EXT_INT_2_CON = 0b010 | (0b010<<4);    /*�½��ش����ж�*/
    EXT_INT_2_MASK = 0;                   /* ȡ�������ⲿ�ж�*/  
 
    VIC0INTENABLE = 0b1<<16;                
    key_VICADDR = (int)key_isr;           /*����key1~key4����ͬһ�ж�Դ������ֻ������һ����ַ*/
    
    __asm__( 
    /*���ж�*/  
    "mrs r0,cpsr\n"
    "bic r0, r0, #0x80\n"
    "msr cpsr_c, r0\n"            
    : 
    : 
  );
}
