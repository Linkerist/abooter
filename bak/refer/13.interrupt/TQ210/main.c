int gboot_main()
{
#ifdef MMU_ON
    mmu_init();
#endif
    led_init();
    button_init();
    init_irq();
    
    while(1);
    
    return 0;    
}

