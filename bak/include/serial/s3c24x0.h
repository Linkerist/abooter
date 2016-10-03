
#ifndef __SERIAL_H__
#define __SERIAL_H__

#define GPHCON (*(volatile unsigned long*)0x56000070)
#define ULCON0 (*(volatile unsigned long*)0x50000000)
#define UCON0  (*(volatile unsigned long*)0x50000004)
#define UBRDIV0  (*(volatile unsigned long*)0x50000028)
#define UTRSTAT0 (*(volatile unsigned long*)0x50000010)
#define UTXH0 (*(volatile unsigned long*)0x50000020)
#define URXH0 (*(volatile unsigned long*)0x50000024)

#define PCLK 50000000
#define BAUD 115200

void uart_init();

void putc(unsigned char ch);

unsigned char getc(void);

void uart_send_string(char *str);

#endif

