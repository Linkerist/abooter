/*
S3C6410��DMA�������裺
1������ʹ�ð�ȫDMAC(SDMAC)����ͨ��DMAC(DMAC)��
2����ʼ��ӦDMAC��ϵͳʱ�ӣ����ر�����һ���ʱ�ӣ�ϵͳĬ�Ͽ���SDMAʱ�ӣ���
3������DMAC���ƣ�����DMAC_Configuration�Ĵ�����
4�������������жϼĴ����ʹ����жϼĴ�����
5��ѡ����ʵ����ȼ�ͨ����
6������ͨ����Դ���ݵ�ַ��Ŀ�����ݵ�ַ������DMACCxSrcAddr��DMACCxDestAddr����
7������ͨ�����ƼĴ���0������DMACCxControl0����
8������ͨ�����ƼĴ���1,�������С������DMACCxControl1����
9������ͨ�����üĴ�����������DMACCxConfiguration��
10��ʹ����Ӧͨ��������DMACCxConfiguratoin����

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
	//DMA��������ѡ��SDMAC0��
	SDMA_SEL = 0;
	
	/* �������Ĺ���ʱ����DMA��������ʱ�Ӳ���ͬ, Ҫʹ��"ͬ���߼�" */
	DMACSync = 0;
	
	//DMA������ʹ��
	DMACConfiguration = 1;
	
	
	//��ʼ��Դ��ַ
	DMACC0SrcAddr = (unsigned int)src;
	
	
	//��ʼ��Ŀ�ĵ�ַ
	DMACC0DestAddr = (unsigned int)UTXH0;
	
	
	
	
	//�Կ��ƼĴ�����������
	/*
	Դ��ַ����
	Ŀ�ĵ�ַ�̶���
	Ŀ������ѡ��AHB����2
	Դ����ѡ��AHB����1
	*/
	DMACC0Control0 =(1<<25) | (1 << 26)| (1<<31);   
	DMACC0Control1 = 0x64;     //����Ĵ�С
	
	
	
	
	
	//����channel0 DMA
	/*
	�����ƺʹ������ͣ�MTP Ϊ 001
	Ŀ�����裺DMA_UART0_1��Դ���裺DMA_MEM
	ͨ����Ч: 1
	*/
	
	DMACC0Configuration = (1<<6) | (1<<11) | (1<<14) | (1<<15);
					
}


void dma_start()
{
	DMACC0Configuration  = 1;	
}
