/*
S3C6410中DMA操作步骤：
1、决定使用安全DMAC(SDMAC)还是通用DMAC(DMAC)；
2、开始相应DMAC的系统时钟，并关闭另外一组的时钟（系统默认开启SDMA时钟）；
3、开启DMAC控制，设置DMAC_Configuration寄存器；
4、清除传输结束中断寄存器和错误中断寄存器；
5、选择合适的优先级通道；
6、设置通道的源数据地址和目的数据地址（设置DMACCxSrcAddr和DMACCxDestAddr）；
7、设置通道控制寄存器0（设置DMACCxControl0）；
8、设置通道控制寄存器1,（传输大小，设置DMACCxControl1）；
9、设置通道配置寄存器；（设置DMACCxConfiguration）
10、使能相应通道（设置DMACCxConfiguratoin）；

*/


#define SDMA_SEL      		(*((volatile unsigned long *)0x7E00F110))
#define DMACIntTCClear      	(*((volatile unsigned long *)0x7DB00008))
#define DMACIntErrClr      	(*((volatile unsigned long *)0x7DB00010))
#define DMACConfiguration      	(*((volatile unsigned long *)0x7DB00030))
#define DMACSync      		(*((volatile unsigned long *)0x7DB00034))
#define DMACC0SrcAddr     	(*((volatile unsigned long *)0x7DB00100))
#define DMACC0DestAddr     	(*((volatile unsigned long *)0x7DB00104))
#define DMACC0Control0      	(*((volatile unsigned long *)0x7DB0010c))
#define DMACC0Control1      	(*((volatile unsigned long *)0x7DB00110))
#define DMACC0Configuration     (*((volatile unsigned long *)0x7DB00114))



#define UTXH0      		(volatile unsigned long *)0x7F005020

char src[100] = "\n\rHello World->";


void dma_init()
{
	//DMA控制器的选择（SDMAC0）
	SDMA_SEL = 0;
	
	/* 如果外设的工作时钟与DMA控制器的时钟不相同, 要使能"同步逻辑" */
	DMACSync = 0;
	
	//DMA控制器使能
	DMACConfiguration = 1;
	
	
	//初始化源地址
	DMACC0SrcAddr = (unsigned int)src;
	
	
	//初始化目的地址
	DMACC0DestAddr = (unsigned int)UTXH0;
	
	
	
	
	//对控制寄存器进行配置
	/*
	源地址自增
	目的地址固定、
	目标主机选择AHB主机2
	源主机选择AHB主机1
	*/
	DMACC0Control0 =(1<<25) | (1 << 26)| (1<<31);   
	DMACC0Control1 = 0x64;     //传输的大小
	
	
	
	
	
	//开启channel0 DMA
	/*
	流控制和传输类型：MTP 为 001
	目标外设：DMA_UART0_1，源外设：DMA_MEM
	通道有效: 1
	*/
	
	DMACC0Configuration = (1<<6) | (1<<11) | (1<<14) | (1<<15);
					
}


void dma_start()
{
	DMACC0Configuration  = 1;	
}
