#define CON (volatile unsigned long*)0xA0000340
#define DAT (volatile unsigned long*)0xA0000344

/* 
 * ���ڶ���������һЩ�궨��
 */ 
#define MMU_FULL_ACCESS     (3 << 10)   /* ����Ȩ�� */
#define MMU_DOMAIN          (0 << 5)    /* �����ĸ��� */
#define MMU_SPECIAL         (1 << 4)    /* ������1 */
#define MMU_CACHEABLE       (1 << 3)    /* cacheable */
#define MMU_BUFFERABLE      (1 << 2)    /* bufferable */
#define MMU_SECTION         (2)         /* ��ʾ���Ƕ������� */
#define MMU_SECDESC         (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | MMU_SECTION)
#define MMU_SECDESC_WB      (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | MMU_CACHEABLE | MMU_BUFFERABLE | MMU_SECTION)


void create_page_table(void)
{
    unsigned long *ttb = (unsigned long *)0x20000000;
    unsigned long vaddr, paddr;

    vaddr = 0xA0000000;
    paddr = 0xE0200000;
    *(ttb + (vaddr >> 20)) = (paddr & 0xFFF00000) | MMU_SECDESC;

    vaddr = 0x20000000;
    paddr = 0x20000000;
    while (vaddr < 0x24000000)
    {
        *(ttb + (vaddr >> 20)) = (paddr & 0xFFF00000) | MMU_SECDESC_WB;
        vaddr += 0x100000;
        paddr += 0x100000;
    }
}


void mmu_init()
{
   __asm__(
    
    /*����TTB*/
    "ldr    r0, =0x20000000\n"                  
    "mcr    p15, 0, r0, c2, c0, 0\n"    
    
    
    /*������Ȩ�޼��*/
    "mvn    r0, #0\n"                   
    "mcr    p15, 0, r0, c3, c0, 0\n"    
    
    
   /*ʹ��MMU*/
    "mrc    p15, 0, r0, c1, c0, 0\n"    
    "orr    r0, r0, #0x0001\n"          
    "mcr    p15, 0, r0, c1, c0, 0\n"    
    : 
    : 
  );
}

int gboot_main()
{
    create_page_table();
    mmu_init();
    
    *(CON) = 0x11110000;
    *(DAT) = 0xA0;
    
    return 0;    
}

