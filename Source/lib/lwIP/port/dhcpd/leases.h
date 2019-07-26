/* leases.h */
#ifndef _LEASES_H
#define _LEASES_H


struct dhcpOfferedAddr {
	u8_t chaddr[16];
	u32_t yiaddr;	/* network order */
	u32_t expires;	/* host order */
};

void clear_lease(u8_t *chaddr, u32_t yiaddr);
struct dhcpOfferedAddr *add_lease(u8_t *chaddr, u32_t yiaddr, unsigned long lease);
int lease_expired(struct dhcpOfferedAddr *lease);
struct dhcpOfferedAddr *oldest_expired_lease(void);
struct dhcpOfferedAddr *find_lease_by_chaddr(u8_t *chaddr);
struct dhcpOfferedAddr *find_lease_by_yiaddr(u32_t yiaddr);
u32_t find_address(int check_expired);
int check_ip(u32_t addr);


#endif
