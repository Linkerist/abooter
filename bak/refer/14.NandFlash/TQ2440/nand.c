#define NFCONF (*(volatile unsigned long*)0x4E000000)
#define NFCONT (*(volatile unsigned long*)0x4E000004)
#define NFSTAT (*(volatile unsigned char*)0x4E000020)
#define NFCMD  (*(volatile unsigned char*)0x4E000008)
#define NFADDR (*(volatile unsigned char*)0x4E00000C)
#define NFDATA (*(volatile unsigned char*)0x4E000010)

#define TACLS 1
#define TWRPH0 2
#define TWRPH1 1


void select_chip()
{
    NFCONT &= ~(1<<1);	
}

void deselect_chip()
{
    NFCONT |= (1<<1);	
}

void clear_RnB()
{
   NFSTAT |= (1<<2); 
}

void send_cmd(unsigned cmd)
{
     NFCMD = cmd;
}

void send_addr(unsigned addr)
{
     NFADDR = addr;
}

void wait_RnB()
{
   while (!(NFSTAT&(1<<2)))	
   {
       ;	
   }
}

void nand_reset()
{
    //ѡ��flash
    select_chip();
    
    //���RnB
    clear_RnB();
    
    
    //����0xff����
    send_cmd(0xff);
    
    
    //�ȴ�RnB
    wait_RnB();
    
	
    //ȡ��ѡ��flash
    deselect_chip();
}

void nand_init()
{
    //��ʼ��NFCONF
    NFCONF = (TACLS<<12) | (TWRPH0<<8) | (TWRPH1<<4);
    
    //��ʼ��NFCONT
    NFCONT = (1<<0) | (1<<1);
    
    //��λ
    nand_reset();	
}

void NF_PageRead(unsigned long addr,unsigned char* buff)
{
	int i;
	
	//ѡ��nandflashоƬ
	select_chip();
	
	//���RnB
	clear_RnB();
	
	//��������0x00
	send_cmd(0x00);
	
	//�����е�ַ
	send_addr(0x00);
	send_addr(0x00);
	
	//�����е�ַ
	send_addr(addr&0xff);
	send_addr((addr>>8)&0xff);
	send_addr((addr>>16)&0xff);
	
	//��������0x30
	send_cmd(0x30);
	
	//�ȴ�RnB
	wait_RnB();
	
	//��ȡ����
	for(i=0;i<2048;i++)
	{
	   buff[i] = NFDATA;  	
	}
	
	//ȡ��ѡ��nandflashоƬ
	deselect_chip();
}


void nand_to_ram(unsigned long start_addr, unsigned char* sdram_addr, int size)
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

void NF_Erase(unsigned long addr)
{
	int ret;
	
        //ѡ��flashоƬ
	select_chip();
	
	//���RnB
	clear_RnB();
	
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
	clear_RnB();
	
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