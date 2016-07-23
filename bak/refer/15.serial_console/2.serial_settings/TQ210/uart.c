#define GPA0CON 		( *((volatile unsigned long *)0xE0200000) )		
#define GPA1CON 		( *((volatile unsigned long *)0xE0200020) )
#define ULCON0 			( *((volatile unsigned long *)0xE2900000) )		
#define UCON0 			( *((volatile unsigned long *)0xE2900004) )
#define UBRDIV0 		( *((volatile unsigned long *)0xE2900028) )
#define UDIVSLOT0 		( *((volatile unsigned long *)0xE290002C) )
#define UTRSTAT0 		( *((volatile unsigned long *)0xE2900010) )
#define UTXH0 			( *((volatile unsigned long *)0xE2900020) )
#define URXH0 			( *((volatile unsigned long *)0xE2900024) )

#define PCLK_PSYS  66000000 // HCLK_PSYS=MPLL/(HCLK_PSYS_RATIO+1);  PCLK_PSYS=HCLK_PSYS/(PCLK_PSYS_RATIO+1); MPLL=667MHZ(参考时钟初始化那个专题)
#define BAUD 115200

void uart_init()
{
    //1.配置引脚功能
	GPA0CON = 0x22222222;
	GPA1CON = 0x2222;
    
    //2.1 设置数据格式
    ULCON0 = 0b11;
    
    //2.2 设置工作模式
    UCON0 = 0b0101; 
    
    //3. 设置波特率	
    UBRDIV0 =(int)(PCLK_PSYS /(BAUD*16)-1);  //UBRDIV0保存该公式计算后的整数部分
    UDIVSLOT0 = 0x0;  //UDISLOT0=保存该公式计算后的小数部分*16
}


void putc(unsigned char ch)
{
    while (!(UTRSTAT0 & (1<<2)));
	UTXH0 = ch;  
}


unsigned char getc(void)
{
    unsigned char ret;

	while (!(UTRSTAT0 & (1<<0)));
	// 取数据
	ret = URXH0;  
	
	if ( (ret == 0x0d) || (ret == 0x0a) )
	{
	    putc(0x0d);
	    putc(0x0a);	
	}      	
	else
	    putc(ret);
	    
        return ret;
}
