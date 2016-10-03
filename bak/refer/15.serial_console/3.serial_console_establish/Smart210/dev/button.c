#define GPH2CON  *((volatile unsigned long *)0xe0200c40)

/*
 * K1,K2,K3,K4∂‘”¶GPH0_1°¢GPH0_2°¢GPH0_3°¢GPH0_4
 */
#define GPH0_int     (0xf<<(0*4))
#define GPH1_int     (0xf<<(1*4))
#define GPH2_int     (0xf<<(2*4))
#define GPH4_int     (0xf<<(4*4))

#define GPH0_msk    (0xf<<(0*4))
#define GPH1_msk    (0xf<<(1*4))
#define GPH2_msk    (0xf<<(2*4))
#define GPH4_msk    (0xf<<(4*4))

void button_init()
{   
    	GPH2CON &= ~(GPH0_msk | GPH1_msk | GPH2_msk | GPH4_msk);
	GPH2CON |= GPH0_int | GPH1_int | GPH2_int | GPH4_int;
}