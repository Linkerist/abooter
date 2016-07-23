
#ifndef __BOOT_H__
#define __BOOT_H__

#define SDRAM_KERNEL_START 0x31000000
#define SDRAM_TAGS_START   0x30000100
#define SDRAM_ADDR_START   0x30000000
#define SDRAM_TOTAL_SIZE   0x04000000

void setup_core_tag();

void setup_mem_tag();

void setup_cmdline_tag();

void setup_end_tag();

void boot_linux();

#endif

