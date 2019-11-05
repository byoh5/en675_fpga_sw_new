/* serverpacket.c
 *
 * Constuct and send DHCP server packets
 *
 * Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "dev.h"

#include <string.h>

#include "lwip/inet.h"
#include "lwip/ip.h"
#include "lwip/pbuf.h"
#include "netif/etharp.h"
#include "lwip/inet_chksum.h"

#include "packet.h"
#include "dhcpd/debug.h"
#include "dhcpd.h"
#include "options.h"
#include "leases.h"

#include "networkif.h"
#include "wifiif.h"

void init_header(struct dhcpMessage *packet, char type)
{
    memset(packet, 0, sizeof(struct dhcpMessage));
    switch (type) {
    case DHCPDISCOVER:
    case DHCPREQUEST:
    case DHCPRELEASE:
    case DHCPINFORM:
        packet->op = BOOTREQUEST;
        break;
    case DHCPOFFER:
    case DHCPACK:
    case DHCPNAK:
        packet->op = BOOTREPLY;
    }
    packet->htype = ETH_10MB;
    packet->hlen = ETH_10MB_LEN;
    packet->cookie = htonl(DHCP_MAGIC);
    packet->options[0] = DHCP_END;
    add_simple_option(packet->options, DHCP_MESSAGE_TYPE, type);
}

/*
    @return : 0 if the packet was sent, else -1 
*/

int raw_packet(struct dhcpd *pdhcpd, u32_t source_ip, int src_port,
               u32_t dest_ip, int dest_port, u8 *dest_arp)
{
    
    ip_addr_t src_ip;
    ip_addr_t dst_ip;
    struct pbuf *q;
    static u32_t raw_ip_id=0;
    struct udp_pcb *pcb;
    
    src_ip.addr = source_ip;
    dst_ip.addr = dest_ip;
    q = pdhcpd->p_out;

    if(q == 0)
    {
        printf("%s, q %p \n",__func__,q);
        goto err;
    }
    /* First, build udp header */
    if(!pbuf_header(q, UDP_HLEN)) /* advand header */
    { 
        struct udp_hdr *udphdr;
        udphdr       = q->payload;
        udphdr->src  = htons(src_port);
        udphdr->dest = htons(dest_port);
        /* in UDP, 0 checksum means 'no checksum' */
        udphdr->chksum = 0x0000; 
        udphdr->len = htons(q->tot_len);
        pcb = pdhcpd->upcb;
    /* calculate checksum */
#if CHECKSUM_GEN_UDP
        if ((pcb->flags & UDP_FLAGS_NOCHKSUM) == 0) {
            udphdr->chksum = inet_chksum_pseudo(q, IP_PROTO_UDP, q->tot_len, &src_ip, &dst_ip);
            /* chksum zero must become 0xffff, as zero means 'no checksum' */
            if (udphdr->chksum == 0x0000) udphdr->chksum = 0xffff;
        }
#endif /* CHECKSUM_CHECK_UDP */
    }
    else
    {
        printf("%s, %d \n",__func__,__LINE__);
        goto err;
    }
    /* Second, build ip header */
    if(!pbuf_header(q,IP_HLEN))
    {
        struct ip_hdr *iphdr;
        struct udp_pcb *pcb;
        
        pcb = pdhcpd->upcb;
        iphdr = q->payload;
        
        IPH_TTL_SET(iphdr, pcb->ttl);
        IPH_PROTO_SET(iphdr,IP_PROTO_UDP);
        ip_addr_set(&(iphdr->dest), &dst_ip);
        IPH_VHL_SET(iphdr, 4, IP_HLEN / 4);
        IPH_TOS_SET(iphdr, pcb->tos);
        IPH_LEN_SET(iphdr, htons(q->tot_len));
        IPH_OFFSET_SET(iphdr, 0);
        IPH_ID_SET(iphdr, htons(raw_ip_id++));
        ip_addr_set(&(iphdr->src), &src_ip);
        IPH_CHKSUM_SET(iphdr, 0);
#if CHECKSUM_GEN_IP
        IPH_CHKSUM_SET(iphdr, inet_chksum(iphdr, IP_HLEN));
#endif
    }
    else
    {   
        printf("%s, %d \n",__func__,__LINE__);
        goto err;
    }            
    /* 3th, build ethnet header */
    if(!pbuf_header(q, sizeof(struct eth_hdr)))
    {
        struct netif *netif = pdhcpd->netif;
        struct eth_hdr *ethhdr = q->payload;
        u8 k;
        k = ETHARP_HWADDR_LEN;
        
        while(k-- > 0)
        {
            ethhdr->dest.addr[k] = dest_arp[k];
            ethhdr->src.addr[k]  = netif->hwaddr[k];
        }
        ethhdr->type = htons(ETHTYPE_IP);
        netif->linkoutput(netif,q);
        pbuf_free(q);
        return 0;
    }
    else
    {
        printf("%s, %d \n",__func__,__LINE__);
        goto err;
    }

err:
    DHCPD_DEBUG(LOG_ERR, "TX pbuf_header ... error");
    if(q)
        pbuf_free(q);
    return -1; 
}

#if 0
/* send a packet to giaddr using the kernel ip stack */
static int send_packet_to_relay(struct dhcpMessage *payload)
{
    DHCPD_DEBUG(LOG_INFO, "Forwarding packet to relay");
	printf("%s(%d) : Forwarding packet to relay\n", __func__, __LINE__);
//  return kernel_packet(payload, server_config.server, SERVER_PORT,
//          payload->giaddr, SERVER_PORT);
	return 0;
	UNUSED(payload);
}
#endif

/* 
    send a packet to a specific arp address and ip address 
    by creating our own ip packet 

    @return 0 if 
*/
static int send_packet_to_client(struct dhcpd *pdhcpd, int force_broadcast)
{
    unsigned char *chaddr;
    struct dhcpMessage *payload = pdhcpd->msg_out;
    u32_t ciaddr;
    
    if (force_broadcast) {
      DHCPD_DEBUG(LOG_INFO, "broadcasting packet to client (NAK)");
      ciaddr = INADDR_BROADCAST;
      chaddr = MAC_BCAST_ADDR;
    } else if (payload->ciaddr) {
      DHCPD_DEBUG(LOG_INFO, "unicasting packet to client ciaddr");
      ciaddr = payload->ciaddr;
      chaddr = payload->chaddr;
    } else if (ntohs(payload->flags) & BROADCAST_FLAG) {
      DHCPD_DEBUG(LOG_INFO, "broadcasting packet to client (requested)");
      ciaddr = INADDR_BROADCAST;
      chaddr = MAC_BCAST_ADDR;
    } else {
      DHCPD_DEBUG(LOG_INFO, "unicasting packet to client yiaddr");
      ciaddr = payload->yiaddr;
      chaddr = payload->chaddr;
    }

    return raw_packet(pdhcpd,server_config.server, SERVER_PORT,
                     ciaddr, CLIENT_PORT, chaddr);        
}


/* send a dhcp packet, if force broadcast is set, the packet will be broadcast to the client */
static int send_packet(struct dhcpd *pdhcpd, int force_broadcast)
{
    int ret;
    ret =  send_packet_to_client(pdhcpd, force_broadcast);
    return ret;
}


static void init_packet(struct dhcpd *pdhcpd, struct dhcpMessage *oldpacket, char type)
{
    pdhcpd->p_out = pbuf_alloc(PBUF_TRANSPORT, sizeof(struct dhcpMessage), PBUF_RAM);
    pdhcpd->msg_out =(struct dhcpMessage*)pdhcpd->p_out->payload;
    init_header(pdhcpd->msg_out, type);
    pdhcpd->msg_out->xid = oldpacket->xid;
    memcpy(pdhcpd->msg_out->chaddr, oldpacket->chaddr, 16);
    pdhcpd->msg_out->flags = oldpacket->flags;
    pdhcpd->msg_out->giaddr = oldpacket->giaddr;
    pdhcpd->msg_out->ciaddr = oldpacket->ciaddr;
    add_simple_option(pdhcpd->msg_out->options, DHCP_SERVER_ID, server_config.server);
}


/* add in the bootp options */
static void add_bootp_options(struct dhcpMessage *packet)
{
    packet->siaddr = server_config.siaddr;
    if (server_config.sname)
        strncpy((char *)packet->sname, server_config.sname, sizeof(packet->sname) - 1);
    if (server_config.boot_file)
        strncpy((char *)packet->file, server_config.boot_file, sizeof(packet->file) - 1);
}
    

/* send a DHCP OFFER to a DHCP DISCOVER */
int sendOffer(struct dhcpd *pdhcpd, struct dhcpMessage *oldpacket)
{
    struct dhcpMessage *packet;
    struct dhcpOfferedAddr *lease = NULL;
    u32_t req_align, lease_time_align = server_config.lease;
    unsigned char *req, *lease_time;
    struct option_set *curr;
    struct in_addr addr;

    init_packet(pdhcpd, oldpacket, DHCPOFFER);  
    packet = pdhcpd->msg_out;
    
    /* ADDME: if static, short circuit */
    /* the client is in our lease/offered table */
    if ((lease = find_lease_by_chaddr(oldpacket->chaddr))) {
        if (!lease_expired(lease)) 
            lease_time_align = lease->expires - timeoffset(0);//time(0);
        packet->yiaddr = lease->yiaddr;
    /* Or the client has a requested ip */
    } else if ((req = get_option(oldpacket, DHCP_REQUESTED_IP)) &&

           /* Don't look here (ugly hackish thing to do) */
           memcpy(&req_align, req, 4) &&

           /* and the ip is in the lease range */
           ntohl(req_align) >= ntohl(server_config.start) &&
           ntohl(req_align) <= ntohl(server_config.end) &&
           
           /* and its not already taken/offered */ /* ADDME: check that its not a static lease */
           ((!(lease = find_lease_by_yiaddr(req_align)) ||
           
           /* or its taken, but expired */ /* ADDME: or maybe in here */
           lease_expired(lease)))) {
                packet->yiaddr = req_align; /* FIXME: oh my, is there a host using this IP? */
    /* otherwise, find a free IP */ /*ADDME: is it a static lease? */
    } else {
        packet->yiaddr = find_address(0);
        /* try for an expired lease */
        if (!packet->yiaddr) packet->yiaddr = find_address(1);
    }
    
    if(!packet->yiaddr) {
        LOG(LOG_WARNING, "no IP addresses to give -- OFFER abandoned");
        return -1;
    }
    
    if (!add_lease(packet->chaddr, packet->yiaddr, server_config.offer_time)) {
        LOG(LOG_WARNING, "lease pool is full -- OFFER abandoned");
        return -1;
    }       

    if ((lease_time = get_option(oldpacket, DHCP_LEASE_TIME))) {
        memcpy(&lease_time_align, lease_time, 4);
        lease_time_align = ntohl(lease_time_align);
        if (lease_time_align > server_config.lease) 
            lease_time_align = server_config.lease;
    }

    /* Make sure we aren't just using the lease time from the previous offer */
    if (lease_time_align < server_config.min_lease) 
        lease_time_align = server_config.lease;
    /* ADDME: end of short circuit */       
    add_simple_option(packet->options, DHCP_LEASE_TIME, htonl(lease_time_align));

    curr = server_config.options;
    while (curr) {
        if (curr->data[OPT_CODE] != DHCP_LEASE_TIME)
            add_option_string(packet->options, curr->data);
        curr = curr->next;
    }

    add_bootp_options(packet);
    
    addr.s_addr = packet->yiaddr;
    LOG(LOG_INFO, "sending OFFER of %s", ipaddr_ntoa((ip_addr_t *)&addr));
    return send_packet(pdhcpd, 0);
}


int sendNAK(struct dhcpd *pdhcpd, struct dhcpMessage *oldpacket)
{
    struct dhcpMessage *packet;
    init_packet(pdhcpd, oldpacket, DHCPNAK);
    packet = pdhcpd->msg_out;
    DHCPD_DEBUG(LOG_INFO, "sending NAK");
    return send_packet(pdhcpd, 1);
}


int sendACK(struct dhcpd *pdhcpd, struct dhcpMessage *oldpacket, u32_t yiaddr)
{
    struct dhcpMessage *packet;
    struct option_set *curr;
    unsigned char *lease_time;
    u32_t lease_time_align = server_config.lease;
    struct in_addr addr;

    init_packet(pdhcpd, oldpacket, DHCPACK);
    packet = pdhcpd->msg_out;
    packet->yiaddr = yiaddr;
    
    if ((lease_time = get_option(oldpacket, DHCP_LEASE_TIME))) {
        memcpy(&lease_time_align, lease_time, 4);
        lease_time_align = ntohl(lease_time_align);
        if (lease_time_align > server_config.lease) 
            lease_time_align = server_config.lease;
        else if (lease_time_align < server_config.min_lease) 
            lease_time_align = server_config.lease;
    }
    
    add_simple_option(packet->options, DHCP_LEASE_TIME, htonl(lease_time_align));
    
    curr = server_config.options;
    while (curr) {
        if (curr->data[OPT_CODE] != DHCP_LEASE_TIME)
            add_option_string(packet->options, curr->data);
        curr = curr->next;
    }

    add_bootp_options(packet);

    addr.s_addr = packet->yiaddr;
    LOG(LOG_INFO, "sending ACK to %s", inet_ntoa(addr));

	netif_state_t *_state = NULL;
#if defined(__ETH__)
	if (pdhcpd->netif == &gnif_eth) {
		_state = &netif_state[enlETHERNET];
	}
#if defined(__WIFI__)
	else
#endif
#endif
#if defined(__WIFI__)
	if (netif_state[enlWIFISTA]._netif && pdhcpd->netif == netif_state[enlWIFISTA]._netif) {
		_state = &netif_state[enlWIFISTA];
	} else if (netif_state[enlWIFIUAP]._netif && pdhcpd->netif == netif_state[enlWIFIUAP]._netif) {
		_state = &netif_state[enlWIFIUAP];
	}
#endif
	if (_state) {
		es_printf("%s DHCPd assigns %IP to the station.\n", _state->ifname, addr.s_addr);
	} else {
		es_printf("%IP DHCPd assigns %IP to the station.\n", pdhcpd->netif->ip_addr.addr, addr.s_addr);
	}
	network_wlifuap_conn_list_setip(packet->chaddr, addr.s_addr);

    if (send_packet(pdhcpd, 0) < 0) 
        return -1;

    add_lease(packet->chaddr, packet->yiaddr, lease_time_align);

    return 0;
}


int send_inform(struct dhcpd *pdhcpd, struct dhcpMessage *oldpacket)
{
    struct dhcpMessage *packet;
    struct option_set *curr;

    init_packet(pdhcpd, oldpacket, DHCPACK);
    packet = pdhcpd->msg_out;
    
    curr = server_config.options;
    while (curr) {
        if (curr->data[OPT_CODE] != DHCP_LEASE_TIME)
            add_option_string(packet->options, curr->data);
        curr = curr->next;
    }

    add_bootp_options(packet);

    return send_packet(pdhcpd, 0);
}



