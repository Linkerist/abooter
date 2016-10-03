
#ifndef __ARP_H__
#define __ARP_H__

#include <linux/types.h>

typedef struct eth_hdr
{
    u8 d_mac[6];
    u8 s_mac[6];
    u16 type;
}ETH_HDR;

typedef struct arp_hdr
{
    ETH_HDR ethhdr;
    u16 hwtype;
    u16 protocol;
    u8 hwlen;
    u8 protolen;
    u16 opcode;
    u8 smac[6];
    u8 sipaddr[4];
    u8 dmac[6];
    u8 dipaddr[4];
}ARP_HDR;

typedef struct ip_hdr
{
    ETH_HDR ethhdr;
    u8 vhl;
    u8 tos;
    u16 len;
    u16 ipid;
    u16 ipoffset;
    u8 ttl;
    u8 proto;
    u16 ipchksum;
    u8 srcipaddr[4];
    u8 destipaddr[4];
}IP_HDR;

typedef struct udp_hdr
{
    IP_HDR iphdr;
    u16 sport;
    u16 dport;
    u16 len;
    u16 udpchksum;
}UDP_HDR;

typedef struct tftp_package
{
    u16 opcode;
    u16 blocknum;
    u8 data[0];	
}TFTP_PAK;

ARP_HDR arpbuf;

#define PROTO_ARP 0x0806
#define PROTO_IP 0x0800
#define PROTO_UDP 0x11

extern u8 host_mac_addr[6];
extern u8 mac_addr[6];
extern u8 ip_addr[4];
extern u8 host_ip_addr[4];
extern u16 packet_len;

void arp_request();

u8 arp_process(u8 *buf, u32 len);

void udp_process(u8* buf, u32 len);

void ip_process(u8 *buf, u32 len);

void net_handle(u8 *buf, u32 len);

#endif

