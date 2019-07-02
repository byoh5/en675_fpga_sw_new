#ifndef __EWL_WEXT_H__
#define __EWL_WEXT_H__

/*    
    @ event --> output  
    
 * @return 1 if an item was successfully received from the queue,
 * otherwise 0.
*/
int 
ewl_wireless_recv_event(struct iw_event  **event);

/*
    @ handle : pointer to wifi device driver (std mode)
    @ wifi wext api interface
    @wrq points to instance of struct iwreq * 
*/
int 
ewl_wext_driver(int handle, void *piwreq, u32 cmd);

#endif
