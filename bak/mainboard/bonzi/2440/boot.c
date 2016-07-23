
#include <linux/atag.h>
#include <linux/string.h>

#include <boot/boot.h>

void (*theKernel)(int , int , unsigned int );

struct tag *pCurTag;

const char *cmdline = "console=ttySAC0,115200 init=/init";

void setup_core_tag()
{
     pCurTag = (struct tag *)SDRAM_TAGS_START;
     
     pCurTag->hdr.tag = ATAG_CORE;
     pCurTag->hdr.size = tag_size(tag_core); 
     
     pCurTag->u.core.flags = 0;
     pCurTag->u.core.pagesize = 4096;
     pCurTag->u.core.rootdev = 0;
     
     pCurTag = tag_next(pCurTag);
}

void setup_mem_tag()
{
     pCurTag->hdr.tag = ATAG_MEM;
     pCurTag->hdr.size = tag_size(tag_mem32); 
     
     pCurTag->u.mem.start = SDRAM_ADDR_START;
     pCurTag->u.mem.size = SDRAM_TOTAL_SIZE;
     
     pCurTag = tag_next(pCurTag);
}

void setup_cmdline_tag()
{
     int linelen = strlen(cmdline);
     
     pCurTag->hdr.tag = ATAG_CMDLINE;
     pCurTag->hdr.size = (sizeof(struct tag_header)+linelen+1+4)>>2;
     
     strcpy(pCurTag->u.cmdline.cmdline,cmdline);
     
     pCurTag = tag_next(pCurTag);
}

void setup_end_tag()
{
	pCurTag->hdr.tag = ATAG_NONE;
	pCurTag->hdr.size = 0;
}

void boot_linux()
{
    //1. ��ȡLinux������ַ
    theKernel = (void (*)(int , int , unsigned int ))SDRAM_KERNEL_START;
    
    //2. ������������
    //2.1 ���ú�����������
    setup_core_tag();
    
    //2.2 �����ڴ����
    setup_mem_tag();
    
    //2.3 ���������в���
    setup_cmdline_tag();
    
    //2.4 ���ò���������־
    setup_end_tag();
    
    //3. ����Linuxϵͳ
    theKernel(0,168,SDRAM_TAGS_START);
}
