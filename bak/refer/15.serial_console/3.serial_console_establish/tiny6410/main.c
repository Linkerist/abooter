
int gboot_main()
{
	int num;
#ifdef MMU_ON
    	mmu_init();
#endif
		
	led_init();
	button_init();
	init_irq();
	 
	led_on();
	
	uart_init();  
	
    	/*
    	putc(0x0d);
    	putc(0x0a);
    	putc('H');
    	*/
    	
	//uart_send_string(buf);
	
	//putc('A');
	
	
		
	while(1)
	{
		//getc();

		printf("\n\r***************************************\n\r");
	    	printf("\n\r*****************GBOOT*****************\n\r");
	    	printf("1:Download Linux Kernel from TFTP Server!\n\r");
	    	printf("2:Boot Linux from RAM!\n\r");
	    	printf("3:Boor Linux from Nand Flash!\n\r");
	    	printf("\n Plese Select:");
	    	
	    	scanf("%d",&num);
	    
	        switch (num)
	        {
	            case 1:
	            //tftp_load();
	            break;
	            
	            case 2:
	            //boot_linux_ram();
	            break;
	            
	            case 3:
	            //boot_linux_nand();
	            break;
	            
	            default:
	                printf("Error: wrong selection!\n\r");
	            break;	
		}
				
	}
  
	
	return 0;    
}


