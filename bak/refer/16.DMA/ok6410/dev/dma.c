/*
S3C6410��DMA�������裺
1������ʹ�ð�ȫDMAC(SDMAC)����ͨ��DMAC(DMAC)��
2������DMAC���ƣ�����DMAC_Configuration�Ĵ�����
3�������������жϼĴ����ʹ����жϼĴ�����
4��ѡ����ʵ����ȼ�ͨ����
5������ͨ����Դ���ݵ�ַ��Ŀ�����ݵ�ַ������DMACC_SrcAddr��DMACC_DestAddr����
6������ͨ�����ƼĴ���0������DMACC_Control0����
7������ͨ�����ƼĴ���1,�������С������DMACC_Control1����
8������ͨ�����üĴ�����������DMACC_Configuration��
9��ʹ����Ӧͨ��������DMACC_Configuratoin����
*/


#define SDMA_SEL      			(*((volatile unsigned long *)0x7E00F110))
#define DMACIntTCClear      	(*((volatile unsigned long *)0x7DB00008))
#define DMACIntErrClr      		(*((volatile unsigned long *)0x7DB00010))
#define DMACConfiguration      	(*((volatile unsigned long *)0x7DB00030))
#define DMACSync     			(*((volatile unsigned long *)0x7DB00034))
#define DMACC0SrcAddr     		(*((volatile unsigned long *)0x7DB00100))
#define DMACC0DestAddr     		(*((volatile unsigned long *)0x7DB00104))
#define DMACC0Control0      	(*((volatile unsigned long *)0x7DB0010c))
#define DMACC0Control1      	(*((volatile unsigned long *)0x7DB00110))
#define DMACC0Configuration     (*((volatile unsigned long *)0x7DB00114))


#define UTXH0      		(volatile unsigned long *)0x7F005020

char src[100] = "\n\rHello World-> This is a test!\n\r";


void dma_init()
{
	//DMA��������ѡ��SDMAC0��
	SDMA_SEL = 0;
	
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
	
	
	
	/*
	�����ƺʹ������ͣ�MTP Ϊ 001
	Ŀ�����裺DMA_UART0_1��Դ���裺DMA_MEM
	ͨ����Ч: 1
	*/

	DMACC0Configuration = (1<<6) | (1<<11) | (1<<14) | (1<<15);

				
}

void dma_start()
{
	//����channel0 DMA
	DMACC0Configuration  = 1;	
}