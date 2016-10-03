.text
.global mem_init
mem_init:
    
    ldr r0, =0x7e00f120
    mov r1, #0x8
    str r1, [r0]

    ldr r0, =0x7e001004  @内存控制命令寄存器
    
    mov r1, #0x4         @根据手册得知需要先进入配置模式
    str r1, [r0]

    ldr r0, =0x7e001010  @刷新寄存器地址
    ldr r1, =( ( 7800 / ( 1000000000/133000000 ) + 1 ) )      @设置刷新时间
    str r1, [r0]

    ldr r0, =0x7e001014  @CAS latency寄存器
    mov r1, #(3 << 1)
    str r1, [r0]

    ldr r0, =0x7e001018  @t_DQSS寄存器
    mov r1, #0x1
    str r1, [r0]

    ldr r0, =0x7e00101c  @T_MRD寄存器
    mov r1, #0x2
    str r1, [r0]

    ldr r0, =0x7e001020   @t_RAS寄存器
    ldr r1, =( ( 45 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e001024   @t_RC寄存器
    ldr r1, =( ( 68 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e001028   @t_RCD寄存器
    ldr r1, =( ( 23 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e00102c   @t_RFC寄存器
    ldr r1, =( ( 80 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e001030   @t_RP寄存器
    ldr r1, =( ( 23 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e001034   @t_rrd寄存器
    ldr r1, =( ( 15 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e001038   @t_wr寄存器
    ldr r1, =( ( 15 / ( 1000000000 / 133000000 ) + 1 ) )
 @  ldr r2, [r0]
    str r1, [r0]

    ldr r0, =0x7e00103c   @t_wtr寄存器
    mov r1, #0x07
    str r1, [r0]

    ldr r0, =0x7e001040   @t_xp寄存器
    mov r1, #0x02
    str r1, [r0]

    ldr r0, =0x7e001044   @t_xsr寄存器
    ldr r1, =( ( 120 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e001048   @t_esr寄存器
    ldr r1, =( ( 120 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e00100c   @内存控制配置寄存器
    ldr r1, =0x00010012   @配置控制器
    str r1, [r0]

    ldr r0, =0x7e00104c   @32位DRAM配置控制寄存器
    ldr r1, =0x0b45
    str r1, [r0]

    ldr r0, =0x7e001200   @片选寄存器
    ldr r1, =0x150f8
    str r1, [r0]

    ldr r0, =0x7e001304   @用户配置寄存器
    mov r1, #0x0
    str r1, [r0]

    ldr r0, =0x7e001008
    ldr r1, =0x000c0000
    str r1, [r0]

    ldr r1, =0x00000000
    str r1, [r0]

    ldr r1, =0x00040000
    str r1, [r0]

    ldr r1, =0x000a0000
    str r1, [r0]

    ldr r1, =0x00080032
    str r1, [r0]

    ldr r0, =0x7e001004
    mov r1, #0x0
    str r1, [r0]

check_dmc1_ready:
    
    ldr r0, =0x7e001000 
    ldr r1, [r0]
    mov r2, #0x3
    and r1, r1, r2
    cmp r1, #0x1
    bne check_dmc1_ready
    nop
    mov pc, lr
