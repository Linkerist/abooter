
#ifndef __LED_H__
#define __LED_H__

#define GPBCON (volatile unsigned long*)0x56000010
#define GPBDAT (volatile unsigned long*)0x56000014

#define GPB5_out        (1<<(5*2))
#define GPB6_out        (1<<(6*2))
#define GPB7_out        (1<<(7*2))
#define GPB8_out        (1<<(8*2))

void led_init();

void led_off();

void led_on();

#endif

