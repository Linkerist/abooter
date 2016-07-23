int gboot_main()
{
#ifdef MMU_ON
    mmu_init();
#endif
    led_init();
    button_init();
    init_irq();
    
    uart_init();
   
    while(1)
        getc();
    
    return 0;    
}


