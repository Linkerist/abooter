#define GPA0CON 		( *((volatile unsigned long *)0xE0200000) )		
#define GPA1CON 		( *((volatile unsigned long *)0xE0200020) )
#define ULCON0 			( *((volatile unsigned long *)0xE2900000) )		
#define UCON0 			( *((volatile unsigned long *)0xE2900004) )
#define UBRDIV0 		( *((volatile unsigned long *)0xE2900028) )
#define UDIVSLOT0 		( *((volatile unsigned long *)0xE290002C) )
#define UTRSTAT0 		( *((volatile unsigned long *)0xE2900010) )
#define UTXH0 			( *((volatile unsigned long *)0xE2900020) )
#define URXH0 			( *((volatile unsigned long *)0xE2900024) )

#define PCLK_PSYS  66000000 // HCLK_PSYS=MPLL/(HCLK_PSYS_RATIO+1);  PCLK_PSYS=HCLK_PSYS/(PCLK_PSYS_RATIO+1); MPLL=667MHZ(�ο�ʱ�ӳ�ʼ���Ǹ�ר��)
#define BAUD 115200

void uart_init()
{
    //1.�������Ź���
	GPA0CON = 0x22222222;
	GPA1CON = 0x2222;
    
    //2.1 �������ݸ�ʽ
    ULCON0 = 0b11;
    
    //2.2 ���ù���ģʽ
    UCON0 = 0b0101; 
    
    //3. ���ò�����	
    UBRDIV0 =(int)(PCLK_PSYS /(BAUD*16)-1);  //UBRDIV0����ù�ʽ��������������
    UDIVSLOT0 = 0x0;  //UDISLOT0=����ù�ʽ������С������*16
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
	// ȡ����
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
