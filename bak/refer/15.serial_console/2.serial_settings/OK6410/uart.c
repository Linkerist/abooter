#define GPACON     	(*((volatile unsigned short *)0x7F008000))
#define ULCON0     	(*((volatile unsigned long *)0x7F005000))
#define UCON0      	(*((volatile unsigned long *)0x7F005004))
#define UTRSTAT0   	(*((volatile unsigned long *)0x7F005010))
#define UBRDIV0    	(*((volatile unsigned short *)0x7F005028))
#define UTXH0      	(*((volatile unsigned char *)0x7F005020))
#define URXH0      	(*((volatile unsigned char *)0x7F005024))
#define UDIVSLOT0  	(*((volatile unsigned short *)0x7F00502C))

#define PCLK 66500000
#define BAUD 115200

void uart_init()
{
    //1.�������Ź���
    GPACON &= ~0xff;
    GPACON |= 0x22;
    
    //2.1 �������ݸ�ʽ
    ULCON0 = 0b11;
    
    //2.2 ���ù���ģʽ
    UCON0 = 0b0101; 
    
    //3. ���ò�����	
    UBRDIV0 =(int)(PCLK/(BAUD*16)-1);  //UBRDIV0����ù�ʽ��������������
    UDIVSLOT0 = 0x0;   //UDISLOT0=����ù�ʽ������С������*16
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
