#ifndef __IF_H__
#define __IF_H__

#define	IFNAMSIZ	16

/*
 * Interface request structure used for socket
 * ioctl's.  All interface ioctl's must have parameter
 * definitions which begin with ifr_name.  The
 * remainder may be interface specific.
 */

struct ifreq {
	union
	{
		char	ifrn_name[IFNAMSIZ];		/* if name, e.g. "en0" */
	} ifr_ifrn;
	
	union {
		char ifru_newname[IFNAMSIZ];
		void *ifru_data;
	} ifr_ifru;
};

#define	ifr_data	ifr_ifru.ifru_data	/* for use by interface	*/
#endif