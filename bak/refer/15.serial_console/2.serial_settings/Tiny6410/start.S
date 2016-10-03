.text
.global _start
_start:
	b reset
	ldr pc, _undifined_instruction
	ldr pc, _software_interrupt
	ldr pc, _prefetch_abort
	ldr pc, _data_abort
	ldr pc, _not_used
	ldr pc, _irq
	ldr pc, _fiq
	

_undifined_instruction: .word undifined_instruction
_software_interrupt: .word software_interrupt
_prefetch_abort: .word prefetch_abort
_data_abort: .word data_abort
_not_used: .word not_used
_irq: .word irq
_fiq: .word reset

undifined_instruction:
	nop

software_interrupt:
	nop
	
prefetch_abort:
	nop
	
data_abort:
	nop

not_used:
	nop

irq:
	nop

fiq:	
	nop

reset:
	bl set_svc
	bl set_peri_port
	bl disable_watchdog
	bl disable_interrupt
	bl disable_mmu
	bl init_clock
	bl mem_init
	bl copy_to_ram
	bl init_stack
	bl clean_bss
	ldr pc, =gboot_main

set_peri_port:
	ldr r0, =0x70000000
	orr r0, r0, #0x13
	mcr p15,0,r0,c15,c2,4
	mov pc, lr

set_svc:
	mrs r0, cpsr
	bic r0, r0,#0x1f
	orr r0, r0,#0xd3
	msr cpsr, r0
	mov pc, lr

#define pWTCON 0x7e004000
disable_watchdog:
	ldr r0, =pWTCON
	mov r1, #0x0
	str r1, [r0]
	mov pc, lr

disable_interrupt:
	mvn r1,#0x0
	ldr r0,=0x71200014
	str r1,[r0]

	ldr r0,=0x71300014
	str r1,[r0]
	mov pc, lr

disable_mmu:
	mcr p15,0,r0,c7,c7,0
	mrc p15,0,r0,c1,c0,0
	bic r0, r0, #0x00000007
	mcr p15,0,r0,c1,c0,0
	mov pc, lr

#define CLK_DIV0 0x7e00f020
#define OTHERS 0x7e00f900
#define MPLL_CON 0x7e00f010
#define APLL_CON 0x7e00f00c
#define CLK_SRC 0x7e00f01c
#define DIV_VAL ((0x0<<0)|(0x1<<9)|(0x1<<8)|(0x3<<12))
#define PLL_VAL ((1<<31)|(266<<16)|(3<<8)|(1<<0))


init_clock:
	ldr r0, =CLK_DIV0
	ldr r1, =DIV_VAL
	str r1, [r0]
	
	ldr r0, =OTHERS
	ldr r1, [r0]
	bic r1,r1,#0xc0
	str r1, [r0]
	
	ldr r0, =APLL_CON
	ldr r1, =PLL_VAL
	str r1, [r0]
	
	ldr r0, =MPLL_CON
	ldr r1, =PLL_VAL
	str r1, [r0]
	
	ldr r0, =CLK_SRC
	mov r1, #0x3
	str r1, [r0]
	
	mov pc, lr
	
copy_to_ram:
	ldr r0, =0x0c000000
	ldr r1, =0x50008000
	add r3, r0, #1024*4

copy_loop:
	ldr r2, [r0], #4
	str r2, [r1], #4
	cmp r0, r3
	bne copy_loop
	mov pc, lr
	
init_stack:
        msr cpsr_c, #0xd2
        ldr sp, =0x53000000 //初始化r13_irq
        msr cpsr_c, #0xd3
	ldr sp, =0x54000000  //初始化R13_svc
	mov pc ,lr

clean_bss:
	ldr r0, =bss_start
	ldr r1, =bss_end
	cmp r0, r1
	moveq pc, lr

clean_loop:
	mov r2, #0
	str r2, [r0], #4
	cmp r0, r1
	bne clean_loop
	mov pc, lr

#define GPKCON 0x7f008800
#define GPKDAT 0x7f008808
light_led:
	ldr r0, =GPKCON
	ldr r1, =0x11110000
	str r1, [r0]
	
	ldr r0, =GPKDAT
	ldr r1, =0xff
	str r1, [r0]
	mov pc, lr
