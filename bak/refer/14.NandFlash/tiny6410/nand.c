/*
tiny6410�õ�nandflashΪ һҳ2K
*/


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
    /* ѡ�� */
    select_ship();
    
    /* ���RnB */
    clean_RnB();
    
    /* ������λ�ź� */
    nand_cmd(0xff);
    
    /* �ȴ����� */
    wait_RnB();
    
    /* ȡ��ѡ�� */
    delselect_ship();
}

void nand_init(void)
{ 

	
    /* ����ʱ����� */
#define TACLS  7
#define TWRPH0 7
#define TWRPH1 7
    
    NFCONF &= ~((7<<12)|(7<<8)|(7<<4));
    NFCONF |= (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
    
    /* ʹ�� nandflash controller*/
    NFCONT = 1 | (1<<1);
    
    
    
    /* ��λ */
    nand_reset();
}

void NF_PageRead(unsigned long addr,unsigned char* buff)
{
    int i;
    
    
    /* ѡ��оƬ */
    select_ship();
    
    /* ���RnB */
    clean_RnB();
    
    /* ��������0x00 */
     nand_cmd(0x00);
        
     /* �����е�ַ */
    nand_addr(0x00);
    nand_addr(0x00);

    /* �����е�ַ */
    nand_addr(addr&0xff);
    nand_addr((addr >>8 ) & (0xff));
    nand_addr((addr >>16 ) & (0xff));
        
    /* ��������0x30 */
     nand_cmd(0x30);
        
    /* �ȴ����� */
     wait_RnB();
        
    /* ������ */
     for(i = 0; i<1024*2; i++)
    {
    	*buff++ = NFDATA;
    }
        
      
     /* ȡ��Ƭѡ */
     
     delselect_ship();
     
}


void nand_to_ram(unsigned long start_addr,unsigned char* sdram_addr,int size)
{
    	int i;
		   	
	for( i=(start_addr >>11); size>0;)
	{
	    NF_PageRead(i,sdram_addr);	
	    size -= 2048;
	    sdram_addr += 2048;
	    i++;
	}
		
}


int NF_Erase(unsigned long addr)
{
	int ret;
	
	//ѡ��flashоƬ
	select_ship();
	
	//���RnB
	clean_RnB();
	
	//��������60
	nand_cmd(0x60);
	
	//�����е�ַ��3�����ڣ�
	nand_addr(addr&0xff);
    	nand_addr((addr >>8 ) & (0xff));
    	nand_addr((addr >>16 ) & (0xff));
	
	//��������D0
	nand_cmd(0xD0);
	
	//�ȴ�RnB
	wait_RnB();
	
	//��������70
	nand_cmd(0x70);
	
	//��ȡ�������
	ret = NFDATA;
	
	//ȡ��ѡ��flashоƬ
	delselect_ship();
	
	return ret;
}

int NF_WritePage(unsigned long addr,unsigned char* buff)
{
	int ret,i;
	
	//ѡ��flashоƬ
	select_ship();
	
	//���RnB
	clean_RnB();
	
	//��������80
	nand_cmd(0x80);
	
	//�����е�ַ��2�����ڣ�
	nand_addr(0x00);
    	nand_addr(0x00);
	
	//�����е�ַ��3�����ڣ�
	nand_addr(addr&0xff);
    	nand_addr((addr >>8 ) & (0xff));
    	nand_addr((addr >>16 ) & (0xff));
	
	//д������
	for(i=0;i<1024*2;i++)
	{
		NFDATA = buff[i];	
	}
	
	//��������10
	nand_cmd(0x10);
	
	//�ȴ�RnB
	wait_RnB();
	
	//��������70
	nand_cmd(0x70);
	
	//��ȡд����
	ret = NFDATA;
	
	//ȡ��ѡ��flashоƬ
	delselect_ship();
	
	return ret;
}