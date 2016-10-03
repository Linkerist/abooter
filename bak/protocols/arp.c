

#include <linux/string.h>

#include <protocols/arp.h>

#include <net/dm9000.h>

#include <lib/printf.h>

#include <protocols/tftp.h>

#define HON(n) ((((u16)((n) & 0xff)) << 8) | (((n) & 0xff00) >> 8))

/*1.发送arp请求包*/
void arp_request()
{
     /*1.构成arp请求包*/
     memcpy(arpbuf.ethhdr.d_mac,host_mac_addr,6);
     memcpy(arpbuf.ethhdr.s_mac,mac_addr,6);
     arpbuf.ethhdr.type = HON(0x0806);
       
     arpbuf.hwtype = HON(1);
     arpbuf.protocol = HON(0x0800);
     
     arpbuf.hwlen = 6;
     arpbuf.protolen = 4;
        
     arpbuf.opcode = HON(1);
 
     memcpy(arpbuf.smac,mac_addr,6);
     memcpy(arpbuf.sipaddr,ip_addr,4);
     memcpy(arpbuf.dipaddr,host_ip_addr,4);

     packet_len = 14+28;
     
     /*2.调用dm9000发送函数，发送应答包*/	
     dm9000_tx((u8 *)&arpbuf,packet_len);
}


/*2.解析arp应答包，提取mac*/
u8 arp_process(u8 *buf, u32 len)
{
    u32 i;
    ARP_HDR *arp_p = (ARP_HDR *)buf;
    
    if (packet_len<28)
        return 0;
    
    switch (HON(arp_p->opcode))
    {
    	case 2: /*arp响应包*/
            memcpy(host_ip_addr,arp_p->sipaddr,4);
            printf("host ip is : ");
            for(i=0;i<4;i++)
               printf("%03d ",host_ip_addr[i]);
            printf("\n\r");
    
            memcpy(host_mac_addr,arp_p->smac,6);
            printf("host mac is : ");
            for(i=0;i<6;i++)
            printf("%02x ",host_mac_addr[i]);
            printf("\n\r");
    
            break;
            
        case 1: /*arp请求包*/
        /*发送arp响应包*/
            memcpy(arpbuf.ethhdr.d_mac,arp_p->smac,6);
            memcpy(arpbuf.ethhdr.s_mac,mac_addr,6);
            arpbuf.ethhdr.type = HON(0x0806);
       
            arpbuf.hwtype = HON(1);
            arpbuf.protocol = HON(0x0800);
     
            arpbuf.hwlen = 6;
            arpbuf.protolen = 4;
        
            arpbuf.opcode = HON(2);
 
            memcpy(arpbuf.smac,mac_addr,6);
            memcpy(arpbuf.sipaddr,ip_addr,4);
            memcpy(arpbuf.dmac,arp_p->smac,6);
            memcpy(arpbuf.dipaddr,arp_p->sipaddr,4);

            packet_len = 14+28;
     
         /*2.调用dm9000发送函数，发送应答包*/	
            dm9000_tx((u8 *)&arpbuf,packet_len);
            
            break;
     }

	return 0;
}

void udp_process(u8* buf, u32 len)
{
     UDP_HDR *udphdr = (UDP_HDR *)buf;
     
     tftp_process(buf,len,HON(udphdr->sport));     	
}

void ip_process(u8 *buf, u32 len)
{
     IP_HDR *p = (IP_HDR *)buf;	
     
     switch(p->proto)
     {
         case PROTO_UDP:
             udp_process(buf,len);
             break;
             
         default:
             break;	
     }
}

void net_handle(u8 *buf, u32 len)
{
     ETH_HDR *p = (ETH_HDR *)buf;
     
     switch (HON(p->type))
     {
          case PROTO_ARP:
         	arp_process(buf,len);
         	break;
          
          case PROTO_IP:
                ip_process(buf,len);
     	        break;
     	  
     	  default:
     	        break;
     }
}
