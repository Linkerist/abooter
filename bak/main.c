
#include <sunshine/led.h>

#include <asm/mach/interrupts.h>
#include <asm/mach/button.h>

#include <serial/s3c24x0.h>
#include <adc/ts.h>
#include <net/dm9000.h>

#include <lib/printf.h>
#include <lib/vsprintf.h>

#include <protocols/tftp.h>
#include <protocols/arp.h>

#include <boot/boot.h>

int __start_main()
{
    int num;
#ifdef MMU_ON
    mmu_init();
#endif
    led_init();
    button_init();
    init_irq();
    
    uart_init();
    
    ts_init();
    
    //lcd_init(); 
    
    //lcd_test();
    
    dm9000_init();
   
    while(1)
    {
    	
	printf("\n***************************************\n\r");
    	printf("\n*****************GBOOT*****************\n\r");
    	printf("1:Download Linux Kernel from TFTP Server!\n\r");
    	printf("2:Boot Linux from RAM!\n\r");
    	printf("3:Send ARP to PC!\n\r");
    	printf("\n Plese Select:");
    	
    	scanf("%d",&num);
    
        switch (num)
        {
            case 1:
            tftp_send_request("zImage");
            break;
            
            case 2:
            boot_linux();
            break;
            
            case 3:
            arp_request();
            break;
            
            default:
                printf("Error: wrong selection!\n\r");
            break;	
        }
    	
    }
    return 0;    
}

