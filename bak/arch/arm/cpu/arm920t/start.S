
.text
.global __start
__start:
	b reset
	ldr pc,__undifined_instruction
	ldr pc,__software_interrupt
	ldr pc,__prefetch_abort
	ldr pc,__data_abort
	ldr pc,__not_used
	ldr pc,__irq
	ldr pc,__fiq
	

__undifined_instruction :
	.word undifined_instruction

__software_interrupt :
	.word software_interrupt

__prefetch_abort :
	.word prefetch_abort

__data_abort :
	.word data_abort

__not_used :
	.word not_used

__irq :
	.word irq

__fiq :
	.word reset


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
	sub lr, lr, #4
	stmfd sp!,{r0-r12,lr}
	bl handle_int
	ldmfd sp!,{r0-r12,pc}^
	
fiq:
	nop

reset:
	bl set_svc
	bl disable_watchdog
	bl disable_interrupt
	bl disable_mmu
	bl init_clock
	bl init_sdram
	bl init_stack
	bl nand_init
	bl copy_to_ram
	bl clean_bss
	ldr pc,=__start_main

@	bl light_led

set_svc:
	mrs r0, cpsr
	bic r0, r0,#0x1f
	orr r0, r0,#0xd3
	msr cpsr, r0
	mov pc, lr

#define pWTCON 0x53000000
disable_watchdog:
	ldr r0, =pWTCON
	mov r1, #0x0
	str r1, [r0]
	mov pc, lr

disable_interrupt:
	mvn r1, #0x0
	ldr r0, =0x4a000008
	str r1, [r0]
	mov pc, lr
	
disable_mmu:
	mcr p15,0,r0,c7,c7,0
	mrc p15,0,r0,c1,c0,0
	bic r0, r0, #0x00000007
	mcr p15,0,r0,c1,c0,0
	mov pc, lr

#define CLKDIVN 0x4c000014
#define MPLLCON 0x4c000004
#define MPLL_405MHZ ((127<<12) | (2<<4) | (1<<0))

init_clock:
	ldr r0, =CLKDIVN
	mov r1, #0x5
	str r1, [r0]
	
	mcr p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000
	mcr p15,0,r0,c1,c0,0
	
	ldr r0, =MPLLCON
	ldr r1, =MPLL_405MHZ
	str r1, [r0]
	mov pc, lr
	
#define mem_contrl 0x48000000
init_sdram:
	ldr r0, =mem_contrl 
	add r3, r0,#(4*13)
	adrl r1, mem_data

0:
	ldr r2, [r1], #4
	str r2, [r0], #4
	cmp r0, r3
	bne 0b
	mov pc, lr
	
copy_to_ram:
	mov r0, #0
	ldr r1, =__start
	ldr r2, =bss_end
	sub r2,r2,r1
	
	mov ip,lr
	bl nand_to_ram
	mov lr, ip
	mov pc,lr
	
init_stack:
	msr cpsr_c,#0xd2
	ldr sp, =0x33000000	@r13_irq actually
	msr cpsr_c,#0xd3
	ldr sp, =0x34000000	@r13_svc actually
	mov pc, lr

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

mem_data:
	.long 0x22000000 
	.long 0x00000700
	.long 0x00000700
	.long 0x00000700
	.long 0x00000700
	.long 0x00000700
	.long 0x00000700
	.long 0x00018001
	.long 0x00018001
	.long 0x008c04f5
	.long 0x000000b1
	.long 0x00000030
	.long 0x00000030

#define GPBCON 0x56000010
#define GPBDAT 0x56000014
light_led:
	ldr r0, =GPBCON
	ldr r1,=0x15400
	str r1, [r0]
	
	ldr r0, =GPBDAT
	ldr r1,=0x6BF
	str r1, [r0]
	mov pc, lr

