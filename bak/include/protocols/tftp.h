
#ifndef __TFTP_H__
#define __TFTP_H__

#define SDRAM_KERNEL_START 0x31000000

#define HON(n) ((((u16)((n) & 0xff)) << 8) | (((n) & 0xff00) >> 8))

u16 checksum(u8 *ptr, int len);

void tftp_send_request(const char *filename);

void tftp_send_ack(u16 blocknum);

void tftp_process(u8 *buf, u32 len, u16 port);

#endif

