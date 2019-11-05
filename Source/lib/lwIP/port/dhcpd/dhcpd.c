#include "dev.h"

#include <stddef.h>
#include <string.h>

#include "enx_freertos.h"
#include "enx_lwip.h"

#include "dhcpd/debug.h"
#include "dhcpd.h"
#include "options.h"
#include "files.h"
#include "serverpacket.h"

#include "networkif.h"

/* globals */
struct dhcpOfferedAddr *leases;
struct server_config_t server_config;

//10


/*******************************************************************************
 Name         : dhcp_process_msg
 Description  : process dhcp msg :
                return 0 if an answer has been prepared
*******************************************************************************/
int dhcpd_handle_packet(struct dhcpd *pdhcpd, struct dhcpMessage *packet)
{
    u8 *state;
    struct dhcpOfferedAddr *lease;
    u8 *server_id, *requested;
    u32 server_id_align, requested_align;
    
    if ((state = get_option(packet, DHCP_MESSAGE_TYPE)) == NULL) {
        DHCPD_DEBUG(LOG_ERR, "couldn't get option from packet, ignoring");
        goto err;
    }   
    
    
    /* ADDME: look for a static lease */
    lease = find_lease_by_chaddr(packet->chaddr);

    switch(state[0])
    {
        case DHCPDISCOVER:                                                     
            DHCPD_DEBUG(LOG_INFO,"received DISCOVER");
            if (sendOffer(pdhcpd, packet) < 0) 
                LOG(LOG_ERR, "send OFFER failed");
            break;
            
        case DHCPREQUEST:
            DHCPD_DEBUG(LOG_INFO, "received REQUEST");
            requested = get_option(packet, DHCP_REQUESTED_IP);
            server_id = get_option(packet, DHCP_SERVER_ID);
            
            if (requested) memcpy(&requested_align, requested, 4);
            if (server_id) memcpy(&server_id_align, server_id, 4);
            
            if (lease) 
            { /*ADDME: or static lease */
                if (server_id) 
                {
                    /* SELECTING State */
                    DHCPD_DEBUG(LOG_INFO, "server_id = %08x", ntohl(server_id_align));
                    if (server_id_align == server_config.server && requested && 
                        requested_align == lease->yiaddr) 
                    {
                        sendACK(pdhcpd,packet,lease->yiaddr);
                    }
                } 
                else 
                {
                    if (requested) 
                    {
                        /* INIT-REBOOT State */
                        if (lease->yiaddr == requested_align)
                            sendACK(pdhcpd,packet, lease->yiaddr);
                        else 
                            sendNAK(pdhcpd,packet);
                    } else 
                    {
                        /* RENEWING or REBINDING State */
                        if (lease->yiaddr == packet->ciaddr)
                            sendACK(pdhcpd,packet, lease->yiaddr);
                        else
                        {
                            /* don't know what to do!!!! */
                            sendNAK(pdhcpd,packet);
                        }
                    }
                }
                /* what to do if we have no record of the client */
            }
            else if (server_id)
            {
                /* SELECTING State */
            }
            else if (requested)
            {
                /* INIT-REBOOT State */
                if ((lease = find_lease_by_yiaddr(requested_align)))
                {
                    if (lease_expired(lease))
                    {
                        /* probably best if we drop this lease */
                        memset(lease->chaddr, 0, 16);
                        /* make some contention for this address */
                    }
                    else
                        sendNAK(pdhcpd,packet);
                } else if (requested_align < server_config.start || 
                       requested_align > server_config.end)
                {
                    sendNAK(pdhcpd,packet);
                } /* else remain silent */ else {
                    sendNAK(pdhcpd,packet); // $CMT-hjlee-180604 - 재부팅시 Request에 대한 NAK 추가
               	}
            } 
            else 
            {
                /* RENEWING or REBINDING State */
            }
            break; 

        case DHCPDECLINE:
            DHCPD_DEBUG(LOG_INFO,"received DECLINE");
			if (lease) 
			{
				memset(lease->chaddr, 0, 16);
				lease->expires = server_config.decline_time + timeoffset(0);//time(0)
			}			
            break;

        case DHCPRELEASE:
            DHCPD_DEBUG(LOG_INFO,"received RELEASE");
			if (lease) lease->expires = timeoffset(0);//time(0)
			break;

        case DHCPINFORM:
			DHCPD_DEBUG(LOG_INFO,"received INFORM");
			send_inform(pdhcpd,packet);
            break;
        default :
            LOG(LOG_WARNING, "unsupported DHCP message (%02x) -- ignoring", state[0]);
            goto err;
            break;      
    }
    return 0;
err:    
    return -1;          
}       
    
/*      
    @ip   : received packet ip.
    @port :                 port.
*/
static void 
dhcpd_recv_cb(void *priv, struct udp_pcb *pcb, struct pbuf *p,
                                     const ip_addr_t *ip, u16_t port) 
{
    struct dhcpd *dhcp = (struct dhcpd *)priv;
    struct dhcpMessage *dhcpd_pkt = (struct dhcpMessage *)(&dhcp->msg_in);
    /* prevent warnings about unused arguments */
    LWIP_UNUSED_ARG(pcb);
    LWIP_UNUSED_ARG(ip);
    LWIP_UNUSED_ARG(port);

    if(!netif_is_up(dhcp->netif))
        goto err;
    
    memset(&dhcp->msg_in,0,sizeof(dhcp->msg_in));
    memcpy(&dhcp->msg_in,p->payload,p->len);
    
    /* if msg is too short, */
    if(p->len < offsetof(struct dhcpMessage,options))
    {
        DHCPD_DEBUG(LOG_INFO,"Message truncated (length was %d)\n",p->len);
        goto err;
    }
    
    if(dhcpd_pkt->op != BOOTREQUEST)
    {
        DHCPD_DEBUG(LOG_ERR, "dhcpd request %d can't be processed\n", dhcpd_pkt->op);
        goto err;
    }
    
    /* process dhcp frame .*/        
    dhcpd_handle_packet(dhcp,dhcpd_pkt);

err:
    if(p)
        pbuf_free(p);
} 

int
dhcpd_start(struct netif *netif)
{
    struct dhcpd *dhcpd;
    struct option_set *option;
    
    memset(&server_config, 0, sizeof(struct server_config_t));
    /* default parameter */
    read_config();
    
    /*read our ip address */
    server_config.server = netif->ip_addr.addr;
    
    if ((option = find_option(server_config.options, DHCP_LEASE_TIME))) {
        memcpy(&server_config.lease, option->data + 2, 4);
        server_config.lease = ntohl(server_config.lease);
        LOG(LOG_INFO,"server_config.lease %d\n", server_config.lease);
    }
    else 
        server_config.lease = LEASE_TIME;
	LOG(LOG_INFO, "1server_config.lease %d\n",server_config.lease);
    if(server_config.lease > LEASE_TIME)
		server_config.lease = LEASE_TIME;
	LOG(LOG_INFO, "2server_config.lease %d\n",server_config.lease);
    leases = pvPortMalloc(sizeof(struct dhcpOfferedAddr) * server_config.max_leases);
    memset(leases, 0, sizeof(struct dhcpOfferedAddr) * server_config.max_leases);    
    
    dhcpd = pvPortCalloc(1,sizeof(struct dhcpd));
    if (dhcpd == NULL) {
        DHCPD_DEBUG(LOG_ERR, "DHCPD [-]: could not allocate memory for dhcpd\n");
        return -1;
    }
    dhcpd->netif = netif;
	server_config.dhcpd = dhcpd;
    dhcpd->upcb = udp_new();
    
    if (dhcpd->upcb == NULL) {
        DHCPD_DEBUG(LOG_ERR, "DHCPD [%p]: could not allocate pcb\n", dhcpd);
        return -1;
    }

#if IP_SOF_BROADCAST
    dhcpd->upcb->so_options|=SOF_BROADCAST;
#endif /* IP_SOF_BROADCAST */

    udp_bind(dhcpd->upcb, IP_ADDR_ANY, SERVER_PORT);
    udp_recv(dhcpd->upcb, dhcpd_recv_cb, dhcpd); 

	netif_state_t *_state = NULL;
#if defined(__ETH__)
	if (netif == &gnif_eth) {
		_state = &netif_state[enlETHERNET];
	}
#if defined(__WIFI__)
	else
#endif
#endif
#if defined(__WIFI__)
	if (netif_state[enlWIFISTA]._netif && netif == netif_state[enlWIFISTA]._netif) {
		_state = &netif_state[enlWIFISTA];
	} else if (netif_state[enlWIFIUAP]._netif && netif == netif_state[enlWIFIUAP]._netif) {
		_state = &netif_state[enlWIFIUAP];
	}
#endif
	if (_state) {
		printf("%s DHCPd starts.\n", _state->ifname);
	} else {
		es_printf("%IP DHCPd starts.\n", netif->ip_addr.addr);
	}

	return 0;
}

int dhcpd_stop(struct netif *netif)
{
    struct dhcpd *dhcpd;
	if (leases != NULL)
    {
        vPortFree(leases);
	}

	dhcpd = server_config.dhcpd;
    if (dhcpd != NULL)
    {
	    if (dhcpd->upcb)
        {
			udp_disconnect(dhcpd->upcb);
			udp_remove(dhcpd->upcb);
			dhcpd->upcb = NULL;
	    }
		vPortFree(dhcpd);
    }

	netif_state_t *_state = NULL;
#if defined(__ETH__)
	if (netif == &gnif_eth) {
		_state = &netif_state[enlETHERNET];
	}
#if defined(__WIFI__)
	else
#endif
#endif
#if defined(__WIFI__)
	if (netif_state[enlWIFISTA]._netif && netif == netif_state[enlWIFISTA]._netif) {
		_state = &netif_state[enlWIFISTA];
	} else if (netif_state[enlWIFIUAP]._netif && netif == netif_state[enlWIFIUAP]._netif) {
		_state = &netif_state[enlWIFIUAP];
	}
#endif
	if (_state) {
		printf("%s DHCPd stops.\n", _state->ifname);
	} else {
		es_printf("%IP DHCPd stops.\n", netif->ip_addr.addr);
	}

    memset(&server_config, 0, sizeof(struct server_config_t));
    return 0;
	UNUSED(netif);
}
