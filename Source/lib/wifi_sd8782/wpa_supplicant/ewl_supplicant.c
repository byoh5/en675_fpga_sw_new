#include "includes.h"
#include "common.h"
#include "os.h"
#include "ewl.h"
#include "wpa_supplicant_i.h"
#include "wpa_supplicant.h"
#include "config.h"
#include "wpa.h"
#include "wireless.h"
#include "eloop.h"
#include <stdio.h>

static struct wpa_supplicant *supplicant=NULL;

/*
 * Scan all channels.
 * Starts a scan of all WiFi channels allowed in this regulatory
 * domain. The list of allowed channels (the domain) is adapted to the 
 * channels announced as allowed by the first AP heard.

   scan process is maximum 3 sec.
   Since ewl_scan() only starts the scanning process 
   the application should callback funtion to this fucntion
   
    @ scan_cb : callback fucntion to be called when scan is finished.

	@return
	- 0 SUCCESS.
	--1 FAILURE.
*/

void
ewl_scan(void (*scan_cb)(void))
{
    supplicant->scan_req = 2;
	supplicant->conf->ap_scan = 2;
    /* we should find, when scan is finished.
       scan process is finished in 3 sec */
    supplicant->scan_state = 1;
    supplicant->scan_complete_cb = scan_cb;
    wpa_supplicant_req_scan(supplicant, 0, 0);  
}

/**
    called if sta failed to connect AP.
    @ connect_failed : callback fucntion to be called when sta failed to connect AP.
    
*/
void
ewl_connect_failed(void (*connect_failed)(void))
{
    supplicant->connect_failed = connect_failed;
}

/*
    Get the list of currently known networks and scan resuls
    
    Retrieves the list of currently known networks from the drvier.
    To ensure that this list is up-to-date a ewl_scan() call should
    be issued and this function should be called upon that 
    callback function of ewl_scan is called
    @return 
    0          : return no scan lists
    otherwise  : number of scan lists 
*/
int 
ewl_get_scan_results(struct wpa_scan_result **results)
{
    int num_scan = 0;
	if(wpa_supplicant_get_scan_results(supplicant) < 0)
	{
    	return 0;
	}
	num_scan = supplicant->num_scan_results;
    *results = supplicant->scan_results;
    supplicant->scan_results = NULL;
    return num_scan;
}

/**
* This function is used to request supplicant to disassociate with the
* current AP.
* current not working : a bug exits, insted, use ewl_remove_network
*/
void 
ewl_disconnect(void)
{
    //supplicant->reassociate = 0;
    //supplicant->disconnected = 1;
    wpa_supplicant_disassociate(supplicant, REASON_DEAUTH_LEAVING);
}


/*
    This function register wep network to supplicant.

    @return : network id(ssd->id) 
              FAILURE -1 : 
*/

int
ewl_add_netowrk(void)
{  
    struct wpa_config *config = supplicant->conf;
    struct wpa_ssid *ssid;
    
    ssid = wpa_config_add_network(config);
    if (ssid == NULL)
        goto out;
        
    ssid->disabled = 1;
    wpa_config_set_network_defaults(ssid);
    return ssid->id;
out:
    return -1;
}

/*
    user input ssid change into wpa_ssid .
    wpa_ssid'format "ssid"
*/
static void
change_to_ssid(char *ssid, char *temp)
{
    int len = os_strlen(ssid);

    temp[0]='"';
    os_memcpy(temp+1, ssid, len);
    temp[len+1]='"';
    temp[len+2] = 0;
}


/**
    Set a variable in network configuration

    @id     : id that identify network
    @name   : network property 
    @value  : network property value

    @return : 0  = success
              -1 = failure
**/

int
ewl_set_network(int id, char *name, char *value)
{
    struct wpa_ssid *ssid;

    /* cmd: "<network id> <variable name> <value>" */ 
    
    ssid = wpa_config_get_network(supplicant->conf, id);
    if (ssid == NULL) 
    {
        return -1;
    }

    if(!os_strcmp("ssid", name))
    {
        char temp[40];
        change_to_ssid(value,temp);
		if (wpa_config_set(ssid, name, temp, 0) < 0) 
            return -1;   
    }
    else if(!os_strcmp("psk", name))
    {
        char temp[80];
        change_to_ssid(value,temp);
        if (wpa_config_set(ssid, name, temp, 0) < 0) 
            return -1;   
    }
    else
        if (wpa_config_set(ssid, name, value, 0) < 0) 
            return -1;
        
    return 0;
}

/*
    Get all current config network lists  
    @return 
        NULL : nothing
        all current config network lists    
*/
struct wpa_ssid * 
ewl_get_network_lists(void)
{
    struct wpa_ssid *ssid = NULL;
    if(supplicant->conf)
        ssid = supplicant->conf->ssid;
    return ssid;
}

/*
    Get current active network 
    @return 
        NULL : nothing
        current network list    
*/
struct wpa_ssid *
ewl_get_current_network_list(void)
{
    return supplicant->current_ssid;
}

/*
    Attempt to connect to network with a given id.

    call before network(struct wpa_ssid) should be set up.

    @id : network id to be conneted to AP

    @return : 0  = success
              -1  = failure if the network could not be found.
*/

int 
ewl_select_network(int id)
{
    struct wpa_ssid *ssid;

    ssid = wpa_config_get_network(supplicant->conf, id);
    if (ssid == NULL) {
        return -1;
    }

    if (ssid != supplicant->current_ssid && supplicant->current_ssid)
        wpa_supplicant_disassociate(supplicant, REASON_DEAUTH_LEAVING);

    /* Mark all other networks disabled and trigger reassociation */
    ssid =supplicant->conf->ssid;
    while (ssid) {
        ssid->disabled = id != ssid->id;
        ssid = ssid->next;
    }

    supplicant->reassociate = 1;
    wpa_supplicant_req_scan(supplicant, 0, 0);

    return 0;
}

/**
    Try to reassociate if there is no connected network
    or set network(struct wpa_ssid) to be enabled(ssid->diabled = 0). 

    @id : network id to be enabled

    @return : 0  = success
             -1  = failure if the network could not be found.
*/
int 
ewl_enable_network(int id)
{
    struct wpa_ssid *ssid;
    
    ssid = wpa_config_get_network(supplicant->conf, id);
    if (ssid == NULL) {
        return -1;
    }

    if (supplicant->current_ssid == NULL && ssid->disabled) {
        /*
         * Try to reassociate since there is no current configuration
         * and a new network was made available. */
        supplicant->disconnect_cnt = 0; 
        supplicant->reassociate = 1;
        wpa_supplicant_req_scan(supplicant, 0, 0);
    }
    ssid->disabled = 0;

    return 0;
}

/**
    called when disconnect occur.
    This situation occur when wpax password is incorrect.
    
    @param disconnect_cb : callback function.
*/

void 
ewl_set_disconnect_cb(void (*disconnect_cb)(int))
{
    supplicant->disconnect_cb = disconnect_cb;
}


/**
    Try to disassociate if network with id is connected.
    and set network(struct wpa_ssid) to be disabled(ssid->diabled = 1). 

    @id : network id to be enabled

    @return : 0  = success
             -1  = failure if the network could not be found.
*/

int 
wpa_supplicant_ctrl_iface_disable_network(int id)
{
    struct wpa_ssid *ssid;

    ssid = wpa_config_get_network(supplicant->conf, id);
    if (ssid == NULL) {
        return -1;
    }

    if (ssid == supplicant->current_ssid)
        wpa_supplicant_disassociate(supplicant, REASON_DEAUTH_LEAVING);
    ssid->disabled = 1;

    return 0;
}

/**
    Try to disassociate if network with id is connected.
    and remove network. 

    @id : network id to be removed

    @return : 0  = success
             -1  = failure if the network could not be found.

*/
int 
ewl_remove_network(int id)
{
    struct wpa_ssid *ssid=NULL;
    ssid = wpa_config_get_network(supplicant->conf, id);
    if (ssid == supplicant->current_ssid)
    {
        wpa_supplicant_disassociate(supplicant, REASON_DEAUTH_LEAVING);
        ssid->disabled = 1;
    }

    if (ssid && wpa_config_remove_network(supplicant->conf, id) < 0) {
        return -1;
    }

    return 0;
}

/**
    force reassociation
*/
int
ewl_reassociate_network(void)
{
    supplicant->disconnected = 0;
    supplicant->reassociate = 1;
    wpa_supplicant_req_scan(supplicant, 0, 0);
	return 0;
}

int
ewl_supplicant_init(void)
{
    supplicant = wpa_supplicant_init();
	return 0;
}

/**
 * wpa_supplicant_run - Run the %wpa_supplicant main event loop
 *
 * This function starts the main event loop and continues running as long as
 * there are any remaining events. In most cases, this function is running as
 * long as the %wpa_supplicant process in still in use.
 */

void ewl_supplicant_run(void)
{
    eloop_run();
}