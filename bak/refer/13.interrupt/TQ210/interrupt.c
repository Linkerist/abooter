/*interrupt registes*/
#define EXT_INT_0_CON       *((volatile unsigned int *)0xE0200E00)   
#define EXT_INT_0_MASK      *((volatile unsigned int *)0xE0200F00)     
#define VIC0INTSELECT       *((volatile unsigned int *)0xF200000C)   
#define VIC0INTENABLE       *((volatile unsigned int *)0xF2000010)   
#define key1_VICADDR        *((volatile unsigned int *)0xF2000100)   
#define key2_VICADDR        *((volatile unsigned int *)0xF2000104)   
#define VIC0ADDRESS         *((volatile unsigned int *)0xF2000F00)   
#define EXT_INT_0_PEND      *((volatile unsigned int *)0xE0200F40)  

void key1_isr()
{
    __asm__( 
    
    "sub lr, lr, #4\n"  
    "stmfd sp!, {r0-r12, lr}\n"       
    : 
    : 
   );

    led_on();
	
    /* ����ж� */    
    EXT_INT_0_PEND =  ~0x0;  
   VIC0ADDRESS = 0;
   
    __asm__( 
    "ldmfd sp!, {r0-r12, pc}^ \n"       
    : 
    : 
  );
   
}

void key2_isr()
{
    __asm__( 
    
    "sub lr, lr, #4\n"  
    "stmfd sp!, {r0-r12, lr}\n"       
    : 
    : 
  );
    led_off();
	
    /* ����ж� */
    EXT_INT_0_PEND = ~0x0; 
    VIC0ADDRESS = 0;  
    
    __asm__( 
    "ldmfd sp!, {r0-r12, pc}^ \n"       
    : 
    : 
  ); 
}


void init_irq()
{
    EXT_INT_0_CON &= ~(0xFF << 0);  
    EXT_INT_0_CON |= 2 | (2 << 4);            /* ����EXT_INT[0]��EXT_INT[1]Ϊ�½��ش��� */  
    EXT_INT_0_MASK &= ~3;                   /* ȡ�������ⲿ�ж�EXT_INT[0]��EXT_INT[1] */  
 
    VIC0INTENABLE |= 3;                     /* ʹ���ⲿ�ж�EXT_INT[0]��EXT_INT[1] */  
    key1_VICADDR = (int)key1_isr;           /* ��EXT_INT[0]�����жϣ����û�����keyʱ�� 
                                        CPU�ͻ��Զ��Ľ�VIC0VECTADDR0��ֵ����VIC0ADDRESS����ת�������ַȥִ */  
    key2_VICADDR = (int)key2_isr;
    
    __asm__( 
    /*���ж�*/  
    "mrs r0,cpsr\n"
    "bic r0, r0, #0x80\n"
    "msr cpsr_c, r0\n"            
    : 
    : 
  );
}
