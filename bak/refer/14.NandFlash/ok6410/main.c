int gboot_main()
{
	unsigned char buf[1024*4];
#ifdef MMU_ON
    mmu_init();
#endif
	
	uart_init();
    	led_init();
    
    	button_init();
    
    	init_irq();
    
    	led_off();
    
    	
	NF_Erase(128*1+1);
	buf[0] = 100;
	NF_WritePage(128*1+1,buf);
    
	buf[0] = 10;
	NF_PageRead(128*1+1,buf);
    
	if( buf[0] == 100 )
		led_on();
    
   
    	
   	while(1);
	
    
    	return 0;    
}


