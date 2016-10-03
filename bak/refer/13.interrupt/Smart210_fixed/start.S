@****************************
@File:start.S
@
@成都国嵌嵌入式培训中心
@****************************

.text
.global	_start
_start:
		b	reset						
		ldr	pc, _undefined_instruction	
		ldr	pc, _software_interrupt		
		ldr	pc, _prefetch_abort			
		ldr	pc, _data_abort				
		ldr	pc, _not_used				
		ldr	pc, _irq					
		ldr	pc, _fiq

_undefined_instruction: .word undefined_instruction
_software_interrupt:	.word software_interrupt
_prefetch_abort:	.word prefetch_abort
_data_abort:		.word data_abort
_not_used:		.word not_used
_irq:			.word irq
_fiq:			.word fiq					

undefined_instruction:
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
		bl disable_watchdog
		bl disable_interrupt
		bl disable_mmu
		bl init_clock
		bl mem_init
		bl copy_to_ram
		bl init_stack
		bl clean_bss
@		bl gboot_main
		ldr pc, =gboot_main
@		bl light_led

set_svc:
		mrs r0, cpsr
		bic r0, r0, #0x1f
		orr r0, r0, #0xd3
		msr cpsr, r0
		mov pc, lr

#define pWTCON 0xE2700000
disable_watchdog:
		ldr r0, =pWTCON
		mov r1, #0x0
		str r1, [r0]
		mov pc, lr

disable_interrupt:
	mvn r1,#0x0
	ldr r0,=0xf2000014
	str r1,[r0]

	ldr r0,=0xf2100014
	str r1,[r0]

	ldr r0,=0xf2200014
	str r1,[r0]

	ldr r0,=0xf2300014
	str r1,[r0]
	mov pc, lr

disable_mmu:
	mcr p15,0,r0,c7,c5,0
	mcr p15,0,r0,c7,c6,1
	mrc p15,0,r0,c1,c0,0
	bic r0, r0, #0x00000007
	mcr p15,0,r0,c1,c0,0
	mov pc, lr

#define CLK_DIV0 0xe0100300
#define MPLL_CON 0xe0100108
#define APLL_CON 0xe0100100
#define CLK_SRC  0xe0100200

#define DIV_VAL ((0x0<<0)|(4<<4)|(4<<8)|(1<<12)|(3<<16)|(1<<20)|(4<<24)|(1<<28))
#define APLL_VAL ((1<<31)|(3<<8)|(125<<16)|(1<<0))
#define MPLL_VAL ((1<<31)|(12<<8)|(667<<16)|(1<<0))

init_clock:
	ldr r0, =CLK_DIV0
	ldr r1, =DIV_VAL
	str r1, [r0]
	
	ldr r0, =APLL_CON
	ldr r1, =APLL_VAL
	str r1, [r0]
	
	ldr r0, =MPLL_CON
	ldr r1, =MPLL_VAL
	str r1, [r0]
	
	ldr r0, =CLK_SRC
	ldr r1, =0x1111
	str r1, [r0]
	
	mov pc, lr

copy_to_ram:
	ldr r0, =0xd0020010
	ldr r1, =0x20008000
	add r3, r0, #1024*4

copy_loop:
	ldr r2, [r0], #4
	str r2, [r1], #4
	cmp r0, r3
	bne copy_loop
	mov pc, lr

init_stack:
	msr cpsr_c, #0xd2
	ldr sp, =0x23000000	@此处实际设置的是r13_irq			
	msr cpsr_c, #0xd3
	ldr sp, =0x24000000	@此处实际设置的是r13_svc
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

light_led:
	ldr	R0,=0xE0200280      
	mov	R1,#0x1
	str	R1,[R0]             
	ldr	R0,=0xE0200284      
	mov	R1,#0x0                   
	str	R1,[R0]             
	mov pc, lr
