
#ifndef __INT_H__
#define __INT_H__

/*interrupt registes*/
#define SRCPND              (*(volatile unsigned *)0x4A000000)
#define INTMOD              (*(volatile unsigned *)0x4A000004)
#define INTMSK              (*(volatile unsigned *)0x4A000008)
#define PRIORITY            (*(volatile unsigned *)0x4A00000c)
#define INTPND              (*(volatile unsigned *)0x4A000010)
#define INTOFFSET           (*(volatile unsigned *)0x4A000014)
#define SUBSRCPND           (*(volatile unsigned *)0x4A000018)
#define INTSUBMSK           (*(volatile unsigned *)0x4A00001c)

#define EINTMASK            (*(volatile unsigned *)0x560000a4)
#define EINTPEND            (*(volatile unsigned *)0x560000a8)

#define EXTINT0   (*(volatile unsigned *)0x56000088)    //External interrupt control register 0

void init_irq();

void handle_int();

#endif

