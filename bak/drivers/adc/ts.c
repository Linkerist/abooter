
#include <adc/ts.h>

#include <asm/mach/interrupts.h>

#include <lib/printf.h>

int xdata,ydata;

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

