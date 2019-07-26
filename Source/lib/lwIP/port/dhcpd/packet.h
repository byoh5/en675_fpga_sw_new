#ifndef _PACKET_H
#define _PACKET_H


struct dhcpMessage {
	u8_t op;
	u8_t htype;
	u8_t hlen;
	u8_t hops;
	u32_t xid;
	u16_t secs;
	u16_t flags;
	u32_t ciaddr;
	u32_t yiaddr;
	u32_t siaddr;
	u32_t giaddr;
	u8_t chaddr[16];
	u8_t sname[64];
	u8_t file[128];
	u32_t cookie;
	u8_t options[308]; /* 312 - cookie */ 
}__attribute__ ((__packed__));

//struct udp_dhcp_packet {
//	struct iphdr ip;
//	struct udphdr udp;
//	struct dhcpMessage data;
//};
//
//void init_header(struct dhcpMessage *packet, char type);
//int get_packet(struct dhcpMessage *packet, int fd);
//u16 checksum(void *addr, int count);
//int raw_packet(struct dhcpMessage *payload, u32 source_ip, int source_port,
//		   u32 dest_ip, int dest_port, unsigned char *dest_arp, int ifindex);
//int kernel_packet(struct dhcpMessage *payload, u32 source_ip, int source_port,
//		   u32 dest_ip, int dest_port);


#endif
