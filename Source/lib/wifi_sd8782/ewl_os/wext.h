#ifndef __WEXT__H__
#define __WEXT__H__
/*
 * This is how a function handling a Wireless Extension should look
 * like (both get and set, standard and private).
 */
typedef int (*iw_handler)(moal_private * priv, struct iw_request_info *info,
                struct iw_point *dwrq, char *extra); 

#endif
