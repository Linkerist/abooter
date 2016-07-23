
#define ULCON0     (*((volatile unsigned long *)0x7F005000))
#define UCON0      (*((volatile unsigned long *)0x7F005004))
#define UFCON0     (*((volatile unsigned long *)0x7F005008))
#define UMCON0     (*((volatile unsigned long *)0x7F00500C))
#define UTRSTAT0   (*((volatile unsigned long *)0x7F005010))
#define UFSTAT0    (*((volatile unsigned long *)0x7F005018))
#define UTXH0      (*((volatile unsigned char *)0x7F005020))
#define URXH0      (*((volatile unsigned char *)0x7F005024))
#define UBRDIV0    (*((volatile unsigned short *)0x7F005028))
#define UDIVSLOT0  (*((volatile unsigned short *)0x7F00502C))

#define GPACON     (*((volatile unsigned long *)0x7F008000))

#define PCLK 66500000
#define BOUD 115200

void uart_init(void)
{
	GPACON &= ~0xff;
	GPACON |= 0x22;
	
	/* ULCON0 */
	ULCON0 = 0x3;  /* 数据位:8, 无较验, 停止位: 1, 8n1 */
	UCON0  = 0x5;  /* 使能UART发送、接收 */
	UFCON0 = 0x01; /* FIFO ENABLE */

	UMCON0 = 0;
	
	/* 波特率 */
	/* DIV_VAL = (PCLK / (bps x 16 ) ) - 1 
	 * bps = 115200
	 * DIV_VAL = (66000000 / (115200 x 16 ) ) - 1 
	 *         = 35.08
	 */
	UBRDIV0 = (int)(PCLK/(BOUD*16)-1);

	/* x/16 = 0.08
	 * x = 1
	 */
	UDIVSLOT0 = 0x1;
	
}

void putc(unsigned char c)
{
	while (UFSTAT0 & (1<<14)); /* 如果TX FIFO满，等待 */
	UTXH0 = c;                         /* 写数据 */
}


unsigned char getc(void)
{
	unsigned char ret;
	while ((UFSTAT0 & 0x7f) == 0);  /* 如果RX FIFO空，等待 */
	ret = URXH0;                   /* 取数据 */
	if((ret == 0x0d) || (ret == 0x0a))
	{
		putc(0x0d);
		putc(0x0a);
	}
	else
	{
		putc(ret);
	}

	return ret;
}


void uart_send_string(char *str)
{
	while(*str)
	{
		putc(*str++);	
	}
	putc(0x0d);
	putc(0x0a);	
}

