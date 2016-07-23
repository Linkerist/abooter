
#define NFCONF             (*((volatile unsigned long*)0x70200000))
#define NFCONT             (*((volatile unsigned long*)0x70200004))
#define NFCMMD             (*((volatile unsigned char*)0x70200008))
#define NFSTAT             (*((volatile unsigned char*)0x70200028))
#define NFADDR             (*((volatile unsigned char*)0x7020000c))
#define NFDATA             (*((volatile unsigned char*)0x70200010))

void select_ship(void)
{
    NFCONT &= ~(1<<1);	
}

void delselect_ship(void)
{
    NFCONT |= (1<<1);
}

void clean_RnB()
{
    NFSTAT |= (1<<4);
} 
void nand_cmd(unsigned char cmd)
{
    NFCMMD = cmd;   
}

void wait_RnB(void)
{
    while(!(NFSTAT & 0x1));
}

void nand_addr(unsigned char addr)
{
    NFADDR = addr;
}

void nand_reset(void)
{
    /* 选中 */
    select_ship();
    
    /* 清除RnB */
    clean_RnB();
    
    /* 发出复位信号 */
    nand_cmd(0xff);
    
    /* 等待就绪 */
    wait_RnB();
    
    /* 取消选中 */
    delselect_ship();
}

void nand_init(void)
{ 
/*
HCLK的频率为100MHZ，周期就为10ns
TACLS > 0 ns
TWRPH0	> 15ns
TWRPH1 > 5ns
 
TACLS的值 = HCLK x TACLS > 0ns
TWRPH0的值 = HCLK x (TWRPH0 + 1) > 15ns
TWRPH1的值 = HCLK x (TWRPH1 +1) > 5ns 
*/
	
    /* 设置时间参数 */
#define TACLS  1
#define TWRPH0 2
#define TWRPH1 1
    
    NFCONF &= ~((7<<12)|(7<<8)|(7<<4));
    NFCONF |= (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
    
    /* 使能 nandflash controller*/
    NFCONT = 1 | (1<<1);
    
    /* 复位 */
    nand_reset();
}


void NF_PageRead(unsigned long addr,unsigned char* buff)
{
    int i;
    
    
    /* 选中芯片 */
    select_ship();
    
    /* 清除RnB */
    clean_RnB();
    
    
    
    
	    /* 发出命令0x00 */
	     nand_cmd(0x00);
	        
	     /* 发出列地址 */
	    nand_addr(0x00);
	    nand_addr(0x00);
	
	    /* 发出行地址 */
	    nand_addr(addr&0xff);
	    nand_addr((addr >>8 ) & (0xff));
	    nand_addr((addr >>16 ) & (0xff));
	        
	    /* 发出命令0x30 */
	     nand_cmd(0x30);
	        
	    /* 等待就绪 */
	     wait_RnB();
	        
	    /* 读数据 */
	     for(i = 0; i<1024*4; i++)
	    {
	    	buff[i] = NFDATA;
	    }
      
      
     
      
     /* 取消片选 */
     
     delselect_ship();
     
}




void nand_to_ram(unsigned long start_addr,unsigned char* sdram_addr,int size)
{
	/* i为页号、sdram_addr为内存中的位置、size拷贝数据的大小 */
	int i;
	unsigned int page_shift = 12;
	
	/* 页的起始地址给i（i为页号）,这个start_addr包含列地址和行地址，我们只需要页地址（行地址） */
	/*for(i=(start_addr >> 12);size>0;)
	{
		NF_PageRead(i,sdram_addr); 	//读一页的数据
	    	size -= 1024*4;  		//ok6410页的大小是4K
	    	start_addr += 1024*4;
	    	i++;
	}
	*/
	
	/* Read pages */
        for (i = 0; i < 4; i++, sdram_addr+=(1<<(page_shift-1))) 
        {
            NF_PageRead(i,sdram_addr);
        }


        /* Read pages */
        for (i = 4; i < (0x3c000>>page_shift); i++, sdram_addr+=(1<<page_shift))
        {
            NF_PageRead(i,sdram_addr);
        }
	
		   
}


int NF_Erase(unsigned long addr)
{
	int ret;
	
	//选中flash芯片
	select_ship();
	
	//清除RnB
	clean_RnB();
	
	//发送命令60
	nand_cmd(0x60);
	
	//发送行地址（3个周期）
	nand_addr(addr&0xff);
    	nand_addr((addr >>8 ) & (0xff));
    	nand_addr((addr >>16 ) & (0xff));
	
	//发送命令D0
	nand_cmd(0xD0);
	
	//等待RnB
	wait_RnB();
	
	//发送命令70
	nand_cmd(0x70);
	
	//读取擦除结果
	ret = NFDATA;
	
	//取消选中flash芯片
	delselect_ship();
	
	return ret;
}

int NF_WritePage(unsigned long addr,unsigned char* buff)
{
	int ret,i;
	
	//选中flash芯片
	select_ship();
	
	//清除RnB
	clean_RnB();
	
	//发送命令80
	nand_cmd(0x80);
	
	//发送列地址（2个周期）
	nand_addr(0x00);
    	nand_addr(0x00);
	
	//发送行地址（3个周期）
	nand_addr(addr&0xff);
    	nand_addr((addr >>8 ) & (0xff));
    	nand_addr((addr >>16 ) & (0xff));
	
	//写入数据
	for(i=0;i<1024*4;i++)
	{
		NFDATA = buff[i];	
	}
	
	//发送命令10
	nand_cmd(0x10);
	
	//等待RnB
	wait_RnB();
	
	//发送命令70
	nand_cmd(0x70);
	
	//读取写入结果
	ret = NFDATA;
	
	//取消选中flash芯片
	delselect_ship();
	
	return ret;
}
