#ifndef _WOAL_WEXT_H_
#define _WOAL_WEXT_H_

/****************************** TYPES ******************************/

/* Shortcuts */
typedef struct iw_statistics	iwstats;
typedef struct iw_range		    iwrange;
typedef struct iw_param		    iwparam;
typedef struct iw_freq		    iwfreq;
typedef struct iw_quality	    iwqual;
typedef struct iw_priv_args	    iwprivargs;
typedef struct sockaddr		    sockaddr;

/* Structure used for parsing event streams, such as Wireless Events
 * and scan results */
typedef struct stream_descr
{
  char *	end;		/* End of the stream */
  char *	current;	/* Current event in stream of events */
  char *	value;		/* Current value in event */
} stream_descr;


/*
 * Scan state and meta-information, used to decode events...
 */
typedef struct iwscan_state
{
  /* State */
  int			ap_num;		/* Access Point number 1->N */
  int			val_index;	/* Value in table 0->(N-1) */
} iwscan_state;


/* Statistics flags (bitmask in updated) */
#define IW_QUAL_QUAL_UPDATED	0x01	/* Value was updated since last read */
#define IW_QUAL_LEVEL_UPDATED	0x02
#define IW_QUAL_NOISE_UPDATED	0x04
#define IW_QUAL_ALL_UPDATED	    0x07
#define IW_QUAL_DBM		        0x08	/* Level + Noise are dBm */
#define IW_QUAL_QUAL_INVALID	0x10	/* Driver doesn't provide value */
#define IW_QUAL_LEVEL_INVALID	0x20
#define IW_QUAL_NOISE_INVALID	0x40
#define IW_QUAL_RCPI		    0x80	/* Level + Noise are 802.11k RCPI */
#define IW_QUAL_ALL_INVALID	    0x70

/* Frequency flags */
#define IW_FREQ_AUTO            0x00    /* Let the driver decides */
#define IW_FREQ_FIXED           0x01    /* Force a specific value */
/** Custom indiciation message sent to the application layer for WMM changes */
#define WMM_CONFIG_CHANGE_INDICATION  "WMM_CONFIG_CHANGE.indication"
/** Custom event : Port Release */
#define CUS_EVT_PORT_RELEASE		"EVENT=PORT_RELEASE"

/** Add event */
#define IWE_STREAM_ADD_EVENT(i, c, e, w, l) 	iwe_stream_add_event((i), (c), (e), (w), (l))
/** Add point */
#define IWE_STREAM_ADD_POINT(i, c, e, w, p) 	iwe_stream_add_point((i), (c), (e), (w), (p))
/** Add value */
#define IWE_STREAM_ADD_VALUE(i, c, v, e, w, l)  iwe_stream_add_value((i), (c), (v), (e), (w), (l))

      
/*
    this function is only used in ewl_lib.a 
*/
                           
void 
ewl_wireless_send_event(moal_private * priv,
                   unsigned int cmd,
                   union iwreq_data *wrqu,
                   char *extra);
                
int
iw_extract_event_stream(struct stream_descr *stream, struct iw_event *iwe);

void
iw_init_event_stream(struct stream_descr *stream,	char *ata, int len);

int
iw_extract_event_stream(struct stream_descr *stream, struct iw_event *iwe);

int
iw_get_priv_size(int args);
     
int
woal_get_freq(moal_private *priv, struct iw_freq *fwrq);   
/*------------------------------------------------------------------*/
/*
 * Wrapper to extract some Wireless Parameter out of the driver
 * @request : WE ID
 * @pwrq    : Fixed part of the request
 */
static inline int
iw_get_ext(moal_private * priv, int request, struct iwreq *	pwrq)
{
#if 0
    /* Set device name */
    //strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
    /* Do the request */
    int ret;
    ENTER();
   // ret = wext_handle_ioctl(priv, pwrq, request, NULL);
    LEAVE();
    return ret;
#else
	ENTER();
	LEAVE();
	return -1;
	UNUSED(priv);
	UNUSED(request);
	UNUSED(pwrq);
#endif
}



/*------------------------------------------------------------------*/
/*
 * Get information about what private ioctls are supported by the driver
 *
 * Note : there is one danger using this function. If it return 0, you
 * still need to free() the buffer. Beware.
 */
static inline int
iw_get_priv_info(moal_private * priv, iwprivargs **	ppriv_args)
{
    struct iwreq		wrq;
    iwprivargs *		privargs = NULL;	    /* Not allocated yet */
    int			maxpriv = 16;	                /* Minimum for compatibility WE<13 */
    iwprivargs *		newargs;
    int errno;
    /* Some driver may return a very large number of ioctls. Some
    * others a very small number. We now use a dynamic allocation
    * of the array to satisfy everybody. Of course, as we don't know
    * in advance the size of the array, we try various increasing
    * sizes. Jean II */
    do
    {
        /* (Re)allocate the buffer */
        newargs = ewl_os_realloc(privargs, maxpriv * sizeof(privargs[0]));
        if(newargs == NULL)
        {
            PRINTM(MERROR,"%s: Allocation failed\n", __FUNCTION__);
            break;
        }
        privargs = newargs;
        
        /* Ask the driver if it's large enough */
        wrq.u.data.pointer = (caddr_t) privargs;
        wrq.u.data.length = maxpriv;
        wrq.u.data.flags = 0;

        if((errno = iw_get_ext(priv, SIOCGIWPRIV, &wrq)) >= 0)
        {
            /* Success. Pass the buffer by pointer */
            *ppriv_args = privargs;
            /* Return the number of ioctls */
            return(wrq.u.data.length);
        }

        /* Only E2BIG means the buffer was too small, abort on other errors */
        if(errno != E2BIG)
        {
            /* Most likely "not supported". Don't barf. */
            break;
        }
        
        /* Failed. We probably need a bigger buffer. Check if the kernel
        * gave us any hints. */
        if(wrq.u.data.length > maxpriv)
        maxpriv = wrq.u.data.length;
        else
        maxpriv *= 2;
    }
    while(maxpriv < 1000);
    
    /* Cleanup */
    if(privargs)
        ewl_os_free(privargs);
    *ppriv_args = NULL;
    
    return(-1);
}              						  


extern struct iw_handler_def woal_handler_def;
						                                                                                           
#endif /* _WOAL_WEXT_H_ */
