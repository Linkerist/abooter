
#include <adc/ts.h>

#include <asm/mach/interrupts.h>

#include <lib/printf.h>

int xdata,ydata;

void ts_init()
{
    //1. 设置AD转换时钟
    ADCCON = (1<<14) | (49<<6);
    
    //2. 设置中断屏蔽位
    INTMSK = ~(1<<31);
    INTSUBMSK = ~(1<<9);
    
    //3. 进入等待中断的模式
    ADCTSC = 0xd3;
}

void tc_handler()
{
	
    //1. 启动xy坐标自动转换 
    ADCTSC = (1<<2);
    ADCCON |= (1<<0);
    
    //2. 等待转换完成
    while (!(ADCCON & (1<<15)))
    
    //3. 获取坐标
    xdata = ADCDAT0 & 0x3ff;
    ydata = ADCDAT1 & 0x3ff;

    //4. 清除按下中断
    SUBSRCPND |= (1<<9);
    SRCPND = (1<<31);
    INTPND = (1<<31); 

    
    //5. 进入等待弹起中断
    ADCTSC = 0xd3;
    ADCTSC |= (1<<8);
    
    while(1)
    {
        if(SUBSRCPND &(1<<9))	
    	    break;
    }
    
    //6. 清除弹起中断
    SUBSRCPND |= (1<<9);
    SRCPND = (1<<31);
    INTPND = (1<<31); 
    
    printf("x is %d,y is %d\n",xdata,ydata);
    
    //7. 再次进入等待按下中断的状态
    ADCTSC = 0xd3;
}

