int xdata,ydata;

#define ADCCON    (*(volatile unsigned *)0x58000000)   //ADC control
#define ADCTSC    (*(volatile unsigned *)0x58000004)   //ADC touch screen control
#define ADCDLY    (*(volatile unsigned *)0x58000008)   //ADC start or Interval Delay
#define ADCDAT0   (*(volatile unsigned *)0x5800000c)   //ADC conversion data 0
#define ADCDAT1   (*(volatile unsigned *)0x58000010)   //ADC conversion data 1

/*interrupt registes*/
#define SRCPND              (*(volatile unsigned long *)0x4A000000)
#define INTMSK              (*(volatile unsigned long *)0x4A000008)
#define INTOFFSET           (*(volatile unsigned long *)0x4A000014)
#define SUBSRCPND           (*(volatile unsigned long *)0x4A000018)
#define INTSUBMSK           (*(volatile unsigned long *)0x4A00001c)
#define INTPND              (*(volatile unsigned long *)0x4A000010)

void ts_init()
{
    //1. ����ADת��ʱ��
    ADCCON = (1<<14) | (49<<6);
    
    //2. �����ж�����λ
    INTMSK = ~(1<<31);
    INTSUBMSK = ~(1<<9);
    
    //3. ����ȴ��жϵ�ģʽ
    ADCTSC = 0xd3;
}

void tc_handler()
{
	
    //1. ����xy�����Զ�ת�� 
    ADCTSC = (1<<2);
    ADCCON |= (1<<0);
    
    //2. �ȴ�ת�����
    while (!(ADCCON & (1<<15)))
    
    //3. ��ȡ����
    xdata = ADCDAT0 & 0x3ff;
    ydata = ADCDAT1 & 0x3ff;

    //4. ��������ж�
    SUBSRCPND |= (1<<9);
    SRCPND = (1<<31);
    INTPND = (1<<31); 

    
    //5. ����ȴ������ж�
    ADCTSC = 0xd3;
    ADCTSC |= (1<<8);
    
    while(1)
    {
        if(SUBSRCPND &(1<<9))	
    	    break;
    }
    
    //6. ��������ж�
    SUBSRCPND |= (1<<9);
    SRCPND = (1<<31);
    INTPND = (1<<31); 
    
    printf("x is %d,y is %d\n",xdata,ydata);
    
    //7. �ٴν���ȴ������жϵ�״̬
    ADCTSC = 0xd3;
}

