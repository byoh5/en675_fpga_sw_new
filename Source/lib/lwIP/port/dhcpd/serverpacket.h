#ifndef _SERVERPACKET_H
#define _SERVERPACKET_H

int sendOffer(struct dhcpd *pdhcpd, struct dhcpMessage *oldpacket);
int sendNAK(struct dhcpd *pdhcpd, struct dhcpMessage *oldpacket);
int sendACK(struct dhcpd *pdhcpd, struct dhcpMessage *oldpacket, u32 yiaddr);
int send_inform(struct dhcpd *pdhcpd, struct dhcpMessage *oldpacket);

#endif
