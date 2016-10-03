#define NFCONF             (*((volatile unsigned long*)0xB0E00000))
#define NFCONT             (*((volatile unsigned long*)0xB0E00004))
#define NFCMMD             (*((volatile unsigned char*)0xB0E00008))
#define NFSTAT             (*((volatile unsigned char*)0xB0E00028))
#define NFADDR             (*((volatile unsigned char*)0xB0E0000C))
#define NFDATA             (*((volatile unsigned char*)0xB0E00010))

#define MP0_1CON 	   (*((volatile unsigned long *)0xE02002E0))
#define MP0_3CON 	   (*((volatile unsigned long *)0xE0200320))

#define MP0_1PUD 	   (*((volatile unsigned long *)0xE02002E8))
#define MP0_3PUD 	   (*((volatile unsigned long *)0xE0200328))

#define page_size   2048

    /* ����ʱ����� */
#define TACLS  7
#define TWRPH0 7
#define TWRPH1 7

//��Ƭѡ
void select_chip(void)
{
    NFCONT &= ~(1<<1);	
}

//ȡ��Ƭѡ
void deselect_chip(void)
{
    NFCONT |= (1<<1);
}

//���RnB
void clean_RnB()
{
    NFSTAT |= (1<<4);
} 

//������
void send_cmd(unsigned char cmd)
{
    NFCMMD = cmd;   
}


//�ȴ�RnB����
void wait_RnB(void)
{
    while(!(NFSTAT & (1<<4)));
}


//����ַ
void send_addr(unsigned char addr)
{
	NFADDR = 0;			  
	NFADDR = 0; 		
	NFADDR = addr & 0xff;			
	NFADDR = (addr >> 8) & 0xff;
	NFADDR = (addr >> 16) & 0xff;
}


//��λ
void nand_reset(void)
{
    /* ѡ�� */
    select_chip();
    
    /* ���RnB */
    clean_RnB();
    
    /* ������λ�ź� */
    send_cmd(0xff);
    
    /* �ȴ����� */
    wait_RnB();
    
    /* ȡ��ѡ�� */
    deselect_chip();
}


//nandflash��ʼ��
void nand_init(void)
{ 
    // ��������  
    	MP0_1CON &= ~(0xf<<8);
    	MP0_1CON |= (0x3<<8);
    	MP0_1PUD &= ~(0x3<<4);
    	
    	MP0_3CON &= ~0xFFFFFF;
    	MP0_3CON |= 0x22222222;
    	MP0_3PUD &= ~(0x3fff);
    	
    	
    	NFCONF &= ~(0x777F); 
    	NFCONF |= (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4)|(0<<3)|(0<<2)|(1<<1)|(0<<0);
    	
    	NFCONT &= ~(0x707C7);
	NFCONT |= (0x1<<23)|(0x1<<22)|(0<<18)|(0<<17)|(0<<16)|(0<<10)|(0<<9)|(0<<8)|(0<<7)|(0<<6)|(0x2<<1)|(1<<0);
    	
    	
    	NFCONF |= 0x70|0x7700;
    	NFCONT |= 0x03;
    	
    	/* ��λ */
    	nand_reset();
}

void nand_read_page(unsigned long addr ,unsigned char* buff)
{
    int i;
    
    
    /* ѡ��оƬ */
    select_chip();
    
    /* ���RnB */
    clean_RnB();
    
    /* ��������0x00 */
     send_cmd(0x00);
    

    /* ������ַ */
    send_addr(addr);
    
    /* ��������0x30 */
     send_cmd(0x30);
        
    /* �ȴ����� */
     wait_RnB();
        
    /* ������ */
     for(i = 0; i<page_size;)
    {
    	*buff++ = NFDATA;
    	 i++;
    }
          
     /* ȡ��Ƭѡ */
     deselect_chip();
}


void nand_to_ram(unsigned long start_addr, unsigned char* sdram_addr, int size)
{
	int i;
	unsigned int page_shift = 11;
	
	for(i = (start_addr>>page_shift) ; size > 0;)
	{
		nand_read_page(i , sdram_addr);
		size -= page_size;
		sdram_addr += page_size;
		i++;
	}
}

void NF_Erase(unsigned long addr)
{
	int ret;
	
        //ѡ��flashоƬ
	select_chip();
	
	//���RnB
	clean_RnB();
	
	//��������0x60
	send_cmd(0x60);
	
	//�����е�ַ
	send_addr(addr&0xff);
	send_addr((addr>>8)&0xff);
	send_addr((addr>>16)&0xff);
	
	//��������D0
	send_cmd(0xD0);
	
	//�ȴ�RnB
	wait_RnB();
	
	//��������0x70
	send_cmd(0x70);
	
	//��ȡ�������
	ret = NFDATA;
	
	//ȡ��ѡ��flashоƬ
	deselect_chip();
	
	return ret;
	
}

int NF_WritePage(unsigned long addr, unsigned char* buff)
{
	int ret;
	int i;
	
	//ѡ��flashоƬ
	select_chip();
	
	//���RnB
	clean_RnB();
	
	//��������0x80
	send_cmd(0x80);
	
	//�����е�ַ
	send_addr(0x00);
	send_addr(0x00);
	
	//�����е�ַ
	send_addr(addr&0xff);
	send_addr((addr>>8)&0xff);
	send_addr((addr>>16)&0xff);
	
	//д������
	for(i=0;i<2048;i++)
	{
	   NFDATA = buff[i]; 	
	}
	
	//��������0x10
	send_cmd(0x10);
	
	//�ȴ�RnB
	wait_RnB();
	
	//��������0x70
	send_cmd(0x70);
	
	//��ȡд����
	ret = NFDATA;
	
	//ȡ��ѡ��flashоƬ
	deselect_chip();
	
	return ret;	
}
