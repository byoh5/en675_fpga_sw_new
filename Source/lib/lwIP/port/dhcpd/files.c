#include "dev.h"

#include "user.h"

#include <string.h>
#include <stdlib.h>

#include "wifi.h"

#include "lwip/inet.h"

#include "enx_freertos.h"

#include "dhcpd/debug.h"
#include "dhcpd.h"
#include "files.h"
#include "options.h"
#include "leases.h"

#if defined(__WIFI__)

char *strdup(const char *s)
{
    char *result = (char*)pvPortMalloc(strlen(s) + 1);
    
    if (result == (char*)0)
        return (char*)0;
    
    strcpy(result, s);
    return result;
}

/* on these functions, make sure you datatype matches */
static int read_ip(char *line, void *arg)
{
    int retval = 1;
    ip_addr_t *addr = arg;
    if(!ipaddr_aton(line,addr))   
    retval = 0;
	return retval;
}

#if 0
static int read_ip_t(void *arg, ip_addr_t *val)
{
    int retval = 1;
    ip_addr_t *addr = arg;
	if (addr) {

    	addr = val;
	}
    retval = 0;
	return retval;
}
#endif

static int read_str(char *line, void *arg)
{
	char **dest = arg;
	
	if (*dest) vPortFree(*dest);
	*dest = strdup(line);
	
	return 1;
}

static int read_u32(char *line, void *arg)
{
	u32 *dest = arg;
	char *endptr;
	*dest = strtoul(line, &endptr, 0);
	return endptr[0] == '\0';
}

static int read_yn(char *line, void *arg)
{
	char *dest = arg;
	int retval = 1;

	if (!strcasecmp("yes", line))
		*dest = 1;
	else if (!strcasecmp("no", line))
		*dest = 0;
	else retval = 0;
	
	return retval;
}

/* read a dhcp option and add it to opt_list */
static int read_opt(char *line, void *arg)
{
	struct option_set **opt_list = arg;
	char *opt, *val, *endptr;
	struct dhcp_option *option = NULL;
	int retval = 0, length = 0;
	char buffer[256];
	u16 result_u16;
	u32 result_u32;
	int i;

	if (!(opt = strtok(line, " \t="))) return 0;

	for (i = 0; options[i].code; i++)
		if (!strcmp(options[i].name, opt))
			option = &(options[i]);

	if (!option) return 0;

	do {
		val = strtok(NULL, ", \t");
		if (val) {
			length = option_lengths[option->flags & TYPE_MASK];
			retval = 0;
			switch (option->flags & TYPE_MASK) {
			case OPTION_IP:
				retval = read_ip(val, buffer);
				break;
			case OPTION_IP_PAIR:
				retval = read_ip(val, buffer);
				if (!(val = strtok(NULL, ", \t/-"))) retval = 0;
				if (retval) retval = read_ip(val, buffer + 4);
				break;
			case OPTION_STRING:
				length = strlen(val);
				if (length > 0) {
					if (length > 254) length = 254;
					memcpy(buffer, val, length);
					retval = 1;
				}
				break;
			case OPTION_BOOLEAN:
				retval = read_yn(val, buffer);
				break;
			case OPTION_U8:
				buffer[0] = strtoul(val, &endptr, 0);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_U16:
				result_u16 = htons(strtoul(val, &endptr, 0));
				memcpy(buffer, &result_u16, 2);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_S16:
				result_u16 = htons(strtol(val, &endptr, 0));
				memcpy(buffer, &result_u16, 2);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_U32:
				result_u32 = htonl(strtoul(val, &endptr, 0));
				memcpy(buffer, &result_u32, 4);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_S32:
				result_u32 = htonl(strtol(val, &endptr, 0));	
				memcpy(buffer, &result_u32, 4);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_HEX6:
				buffer[0] = val[0];
				buffer[1] = val[1];
				buffer[2] = val[2];
				buffer[3] = val[3];
				buffer[4] = val[4];
				buffer[5] = val[5];
				retval = 1;
			default:
				break;
			}
			if (retval) 
				attach_option(opt_list, option, buffer, length);
		};
	} while (val && retval && option->flags & OPTION_LIST);
	return retval;
}

//#define DHCP_SERVER_IPADDR "192.168.15."
#if 0 
static struct config_keyword keywords[] = {
    /* keyword[14]	handler   variable address		default[20] */
    {"server",      read_ip,  &(server_config.server),   DHCP_SERVER_IPADDR"1"},
    {"start",	    read_ip,  &(server_config.start),	 DHCP_SERVER_IPADDR"2"},
    {"end",		    read_ip,  &(server_config.end),		 DHCP_SERVER_IPADDR"254"},
    {"interface",	read_str, &(server_config.interface),"eth0"},
    {"option",	    read_opt, &(server_config.options),	 "subnet	255.255.255.0"},
    {"option",	    read_opt, &(server_config.options),	 "dns	129.219.13.81 "},
    {"option",	    read_opt, &(server_config.options),	 "domain	local"},
    {"option",	    read_opt, &(server_config.options),	 "lease	864000 "},
    {"opt",		    read_opt, &(server_config.options),	 "dns "DHCP_SERVER_IPADDR"1 "DHCP_SERVER_IPADDR"10"},
    {"opt",		    read_opt, &(server_config.options),	 "router	"DHCP_SERVER_IPADDR"1"},
    {"opt",		    read_opt, &(server_config.options),	 "wins	"DHCP_SERVER_IPADDR"1"},

    {"max_leases",	read_u32, &(server_config.max_leases),   "10"},
    {"remaining",	read_yn,  &(server_config.remaining),    "yes"},
    {"auto_time",	read_u32, &(server_config.auto_time),    "7200"},
    {"decline_time",read_u32, &(server_config.decline_time), "3600"},
    {"conflict_time",read_u32,&(server_config.conflict_time),"3600"},
    {"offer_time",	read_u32, &(server_config.offer_time),	 "60"},
    {"min_lease",	read_u32, &(server_config.min_lease),	 "60"},
    {"siaddr",	    read_ip,  &(server_config.siaddr),	     "0.0.0.0"},
    {"sname",	    read_str, &(server_config.sname),	     ""},
    {"boot_file",	read_str, &(server_config.boot_file),	 ""},
    /*ADDME: static lease */
    {"",		    NULL, 	  NULL,				                 ""}
};
#endif

int read_config(void)
{
	LOG(LOG_INFO, "DHCPd info config\r\n");

	char cfg_localip[16] = {0};
	sprintf(cfg_localip, "%IP", gtNetwork.UAPcfg.un32Localip);

	char cfg_startadd[16] = {0};
	sprintf(cfg_startadd, "%IP", gtNetwork.UAPcfg.un32StartAddress);

	char cfg_endadd[16] = {0};
	sprintf(cfg_endadd, "%IP", gtNetwork.UAPcfg.un32EndAddress);

	char cfg_netmask[16] = {0};
	sprintf(cfg_netmask, "%IP", gtNetwork.UAPcfg.un32Netmask);

	char subnet[50] = {0};
	sprintf(subnet, "subnet	%s", cfg_netmask);

#if 1
	char netbios[50] = {0};
	sprintf(netbios, "netbios	%c%c%c%c%c%c", 1, 4, 0, 0, 0, 2);
#else
	char router[50] = {0};
	sprintf(router, "router	%s", cfg_localip);

	char dnsip[50] = {0};
	sprintf(dnsip, "dns	129.219.13.81");

	char Localip[50] = {0};
	sprintf(Localip, "wins	%s", cfg_localip);

	char dnsadd[50] = {0};
	sprintf(dnsadd, "dns %s%d %s%d", DHCP_SERVER_IPADDR, 1, DHCP_SERVER_IPADDR, 10);
#endif

	char leaseT[50] = {0};
	sprintf(leaseT, "lease	%d", gtNetwork.UAPcfg.leaseTime);

	LOG(LOG_INFO, "AP Config Load Localip: %s\r\n",cfg_localip);
	LOG(LOG_INFO, "AP Config Load StartAdd: %s\r\n",cfg_startadd);
	LOG(LOG_INFO, "AP Config Load EndAdd: %s\r\n",cfg_endadd);

	LOG(LOG_INFO, "AP Config Load subnet: %s\r\n",subnet);
//	LOG(LOG_INFO, "AP Config Load dnsip: %s\r\n",dnsip);
//	LOG(LOG_INFO, "AP Config Load Localip: %s\r\n",Localip);
//	LOG(LOG_INFO, "AP Config Load dnsadd: %s\r\n",dnsadd);
//	LOG(LOG_INFO, "AP Config Load router: %s\r\n",router);

	read_ip(cfg_localip, &server_config.server);
	read_ip(cfg_startadd, &server_config.start);
	read_ip(cfg_endadd, &server_config.end);	
	server_config.lease = gtNetwork.UAPcfg.leaseTime;
	read_str("wl1", &server_config.interface);
	read_opt(subnet, &server_config.options);
	read_opt(leaseT, &server_config.options);
#if 1
	read_opt(netbios, &server_config.options);
#else
	read_opt(router, &server_config.options);
	read_opt(dnsip, &server_config.options);
	read_opt("domain	local", &server_config.options);
	read_opt(dnsadd , &server_config.options);
	read_opt(Localip, &server_config.options);
#endif
	server_config.max_leases = gtNetwork.UAPcfg.un8MaxSTA+5;
    read_yn("yes", server_config.remaining);
    read_u32("7200", &server_config.auto_time);
    read_u32("3600", &server_config.decline_time);
    read_u32("3600", &server_config.conflict_time);
    read_u32("60", &server_config.offer_time);
    read_u32("60", &server_config.min_lease);
    read_ip("0.0.0.0",  &server_config.siaddr);
    read_str("", &server_config.sname);
    read_str("", &server_config.boot_file);
	LOG(LOG_INFO, "AP Config Load #Localip: %s\r\n", ipaddr_ntoa(&server_config.server));
	LOG(LOG_INFO, "AP Config Load #StartAdd: %s\r\n", ipaddr_ntoa(&server_config.start));
	LOG(LOG_INFO, "AP Config Load #EndAdd: %s\r\n", ipaddr_ntoa(&server_config.end));	
	LOG(LOG_INFO, "AP Config Load #leaseTime: %d\r\n", server_config.lease);
	if(server_config.interface)
		LOG(LOG_INFO, "AP Config Load #interface: %s\r\n",server_config.interface);

	struct option_set *opt_list = server_config.options;
	if(0)//opt_list)
	{
		if(opt_list->data)
		if(opt_list->data[OPT_LEN]>0)
		LOG(LOG_INFO, "AP Config Load #options: %s\r\n",opt_list->data[OPT_DATA]);
		if(opt_list->next)
		opt_list = opt_list->next;
		if(opt_list->data)
		if(opt_list->data[OPT_LEN]>0)
		LOG(LOG_INFO, "AP Config Load #options: %s\r\n",opt_list->data[OPT_DATA]);
		if(opt_list->next)
		opt_list = opt_list->next;
		if(opt_list->data)
		if(opt_list->data[OPT_LEN]>0)
		LOG(LOG_INFO, "AP Config Load #options: %s\r\n",opt_list->data[OPT_DATA]);
		if(opt_list->next)
		opt_list = opt_list->next;
		if(opt_list->data)
		if(opt_list->data[OPT_LEN]>0)
		LOG(LOG_INFO, "AP Config Load #options: %s\r\n",opt_list->data[OPT_DATA]);
		if(opt_list->next)
		opt_list = opt_list->next;
		if(opt_list->data)
		if(opt_list->data[OPT_LEN]>0)
		LOG(LOG_INFO, "AP Config Load #options: %s\r\n",opt_list->data[OPT_DATA]);
		if(opt_list->next)
		opt_list = opt_list->next;
		if(opt_list->data)
		if(opt_list->data[OPT_LEN]>0)
		LOG(LOG_INFO, "AP Config Load #options: %s\r\n",opt_list->data[OPT_DATA]);
		if(opt_list->next)
		opt_list = opt_list->next;
		if(opt_list->data)
		if(opt_list->data[OPT_LEN]>0)
		LOG(LOG_INFO, "AP Config Load #options: %s\r\n",opt_list->data[OPT_DATA]);
	}
	LOG(LOG_INFO, "AP Config Load #maxleases: %d\r\n", server_config.max_leases);
//	LOG(LOG_INFO, "AP Config Load #remaining: %d\r\n", *server_config.remaining);
	LOG(LOG_INFO, "AP Config Load #autoTime: %d\r\n", server_config.auto_time);
	LOG(LOG_INFO, "AP Config Load #decline_time: %d\r\n", server_config.decline_time);
	LOG(LOG_INFO, "AP Config Load #conflict_time: %d\r\n", server_config.conflict_time);
	LOG(LOG_INFO, "AP Config Load #offer_time: %d\r\n", server_config.offer_time);
	LOG(LOG_INFO, "AP Config Load #min_lease: %d\r\n", server_config.min_lease);
	LOG(LOG_INFO, "AP Config Load #min_lease: %s\r\n", ipaddr_ntoa(&server_config.siaddr));

	return 1;
}
#endif
