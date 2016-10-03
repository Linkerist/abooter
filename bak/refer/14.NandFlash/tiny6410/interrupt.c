/*interrupt registes*/
#define EXT_INT_0_CON       *((volatile unsigned int *)0x7f008900)   
#define EXT_INT_1_CON       *((volatile unsigned int *)0x7f008904)  
#define EXT_INT_0_MASK      *((volatile unsigned int *)0x7f008920) 
#define EXT_INT_0_PEND      *((volatile unsigned int *)0x7f008924)     

#define VIC0INTENABLE       *((volatile unsigned int *)0x71200010)   
#define VIC1INTENABLE       *((volatile unsigned int *)0x71300010)

#define EINT0_VECTADDR      *((volatile unsigned int *)0x71200100)     
#define EINT20_VECTADDR     *((volatile unsigned int *)0x71300104) 
#define VIC0ADDRESS         *((volatile unsigned int *)0x71200f00)   
#define VIC1ADDRESS         *((volatile unsigned int *)0x71300f00)

void key1_handle()
{
    __asm__( 
    
    "sub lr, lr, #4\n"  
    "stmfd sp!, {r0-r12, lr}\n"       
    : 
    : 
   );

    led_on();
	
    /* 清除中断 */
    EXT_INT_0_PEND = ~0x0;  
    VIC0ADDRESS = 0; 
    VIC1ADDRESS = 0; 
   
    __asm__( 
    "ldmfd sp!, {r0-r12, pc}^ \n"       
    : 
    : 
  );
   
}

void key8_handle()
{
    __asm__( 
    
    "sub lr, lr, #4\n"  
    "stmfd sp!, {r0-r12, lr}\n"       
    : 
    : 
  );
    led_off();
	
    /* 清除中断 */
    EXT_INT_0_PEND = ~0x0; 
    VIC0ADDRESS = 0; 
    VIC1ADDRESS = 0;   
    
    __asm__( 
    "ldmfd sp!, {r0-r12, pc}^ \n"       
    : 
    : 
  ); 
}

void init_irq()
{ 
    EXT_INT_0_CON = 0b010;            /* 配置为下降沿触发 */  
    EXT_INT_1_CON = 0x010<<8;            /* 配置为下降沿触发 */ 
    
    EXT_INT_0_MASK = 0;                   /* 取消屏蔽外部中断 */  
 
    VIC0INTENABLE |= 0x1;                     /* 使能外部中断*/  
    VIC1INTENABLE |= 0x0f;
    
    EINT0_VECTADDR = (int)key1_handle;           /* 用户按下key时，CPU就会自动的将VIC0VECTADDR0的值赋给VIC0ADDRESS并跳转到这个地址去执 */  
    EINT20_VECTADDR = (int)key8_handle;
   
    __asm__( 
    
    "mrc p15,0,r0,c1,c0,0\n"
    "orr r0,r0,#(1<<24)\n"
    "mcr p15,0,r0,c1,c0,0\n"

    "mrs r0,cpsr\n"
    "bic r0, r0, #0x80\n"
    "msr cpsr_c, r0\n"            
    : 
    : 
  );
}