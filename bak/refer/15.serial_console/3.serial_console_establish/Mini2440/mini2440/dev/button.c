#define GPGCON  (volatile unsigned long *)0x56000060

/*
 * K1,K2,K3,K4∂‘”¶GPG0°¢GPG3°¢GPG5°¢GPG6
 */
#define GPG0_int     (0x2<<(0*2))
#define GPG3_int     (0x2<<(3*2))
#define GPG5_int     (0x2<<(5*2))
#define GPG6_int     (0x2<<(6*2))

#define GPG0_msk    (3<<(0*2))
#define GPG3_msk    (3<<(3*2))
#define GPG5_msk    (3<<(5*2))
#define GPG6_msk    (3<<(6*2))

void button_init()
{   
    	*(GPGCON) &= ~(GPG0_msk | GPG3_msk | GPG5_msk | GPG6_msk);
	*(GPGCON) |= GPG0_int | GPG3_int | GPG5_int | GPG6_int;
}