
#include <ewl_os.h>
#include <errno.h> //for ENOMEM
                                                                          
#include "wireless.h"                                                         
#include "moal_main.h"                          
#include "moal_priv.h"                          
#include "moal_util.h"                          
#include "moal_wext.h"  
#include "wext.h"                        
#include "iw_handler.h"  
#include "ewl.h"

static const struct 
iw_ioctl_description standard_ioctl[] = 
{

    [SIOCSIWCOMMIT  - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_NULL,
    },

    [SIOCGIWNAME    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_CHAR,
        .flags      = IW_DESCR_FLAG_DUMP,
    },

    [SIOCSIWNWID    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
        .flags      = IW_DESCR_FLAG_EVENT,
    },

    [SIOCGIWNWID    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
        .flags      = IW_DESCR_FLAG_DUMP,
    },

    [SIOCSIWFREQ    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_FREQ,
        .flags      = IW_DESCR_FLAG_EVENT,
    },

    [SIOCGIWFREQ    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_FREQ,
        .flags      = IW_DESCR_FLAG_DUMP,
    },

    [SIOCSIWMODE    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_UINT,
        .flags      = IW_DESCR_FLAG_EVENT,
    },

    [SIOCGIWMODE    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_UINT,
        .flags      = IW_DESCR_FLAG_DUMP,
    },

    [SIOCSIWSENS    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCGIWSENS    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCSIWRANGE   - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_NULL,
    },

    [SIOCGIWRANGE   - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = sizeof(struct iw_range),
        .flags      = IW_DESCR_FLAG_DUMP,
    },

    [SIOCSIWPRIV    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_NULL,
    },

    [SIOCGIWPRIV    - SIOCIWFIRST] = { /* (handled directly by us) */
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = sizeof(struct iw_priv_args),
        .max_tokens = 16,
        .flags      = IW_DESCR_FLAG_NOMAX,
    },

    [SIOCSIWSTATS   - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_NULL,
    },

    [SIOCGIWSTATS   - SIOCIWFIRST] = { /* (handled directly by us) */
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = sizeof(struct iw_statistics),
        .flags      = IW_DESCR_FLAG_DUMP,
    },

    [SIOCSIWAP  - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_ADDR,
    },

    [SIOCGIWAP  - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_ADDR,
        .flags      = IW_DESCR_FLAG_DUMP,
    },

    [SIOCSIWMLME    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .min_tokens = sizeof(struct iw_mlme),
        .max_tokens = sizeof(struct iw_mlme),
    },

    [SIOCSIWSCAN    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .min_tokens = 0,
        .max_tokens = sizeof(struct iw_scan_req),
    },

    [SIOCGIWSCAN    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = IW_SCAN_MAX_DATA,
        .flags      = IW_DESCR_FLAG_NOMAX,
    },

    [SIOCSIWESSID   - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = IW_ESSID_MAX_SIZE,
        .flags      = IW_DESCR_FLAG_EVENT,
    },

    [SIOCGIWESSID   - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = IW_ESSID_MAX_SIZE,
        .flags      = IW_DESCR_FLAG_DUMP,
    },

    [SIOCSIWNICKN   - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = IW_ESSID_MAX_SIZE,
    },

    [SIOCGIWNICKN   - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = IW_ESSID_MAX_SIZE,
    },

    [SIOCSIWRATE    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCGIWRATE    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCSIWRTS - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCGIWRTS - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCSIWFRAG    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCGIWFRAG    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCSIWTXPOW   - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCGIWTXPOW   - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCSIWRETRY   - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCGIWRETRY   - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCSIWENCODE  - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = IW_ENCODING_TOKEN_MAX,
        .flags      = IW_DESCR_FLAG_EVENT | IW_DESCR_FLAG_RESTRICT,
    },

    [SIOCGIWENCODE  - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = IW_ENCODING_TOKEN_MAX,
        .flags      = IW_DESCR_FLAG_DUMP | IW_DESCR_FLAG_RESTRICT,
    },

    [SIOCSIWPOWER   - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCGIWPOWER   - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCSIWGENIE   - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = IW_GENERIC_IE_MAX,
    },

    [SIOCGIWGENIE   - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = IW_GENERIC_IE_MAX,
    },

    [SIOCSIWAUTH    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCGIWAUTH    - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_PARAM,
    },

    [SIOCSIWENCODEEXT - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .min_tokens = sizeof(struct iw_encode_ext),
        .max_tokens = sizeof(struct iw_encode_ext) +
                  IW_ENCODING_TOKEN_MAX,
    },

    [SIOCGIWENCODEEXT - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .min_tokens = sizeof(struct iw_encode_ext),
        .max_tokens = sizeof(struct iw_encode_ext) +
                  IW_ENCODING_TOKEN_MAX,
    },

    [SIOCSIWPMKSA - SIOCIWFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .min_tokens = sizeof(struct iw_pmksa),
        .max_tokens = sizeof(struct iw_pmksa),
    },
};

static const unsigned 
standard_ioctl_num =  sizeof(standard_ioctl)/sizeof(struct iw_ioctl_description);

/* Size (in bytes) of various events */
static const int 
event_type_size[] = 
{
    IW_EV_LCP_LEN,          /* IW_HEADER_TYPE_NULL */
    0,
    IW_EV_CHAR_LEN,         /* IW_HEADER_TYPE_CHAR */
    0,
    IW_EV_UINT_LEN,         /* IW_HEADER_TYPE_UINT */
    IW_EV_FREQ_LEN,         /* IW_HEADER_TYPE_FREQ */
    IW_EV_ADDR_LEN,         /* IW_HEADER_TYPE_ADDR */
    0,
    IW_EV_POINT_LEN,        /* Without variable payload */
    IW_EV_PARAM_LEN,        /* IW_HEADER_TYPE_PARAM */
    IW_EV_QUAL_LEN,         /* IW_HEADER_TYPE_QUAL */

};


/*
 * Meta-data about all the additional standard Wireless Extension events
 * we know about.
 */
static const struct 
iw_ioctl_description standard_event[] = 
{
    [IWEVTXDROP - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_ADDR,
    },
    [IWEVQUAL   - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_QUAL,
    },
    [IWEVCUSTOM - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = IW_CUSTOM_MAX,
    },
    [IWEVREGISTERED - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_ADDR,
    },
    [IWEVEXPIRED    - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_ADDR,
    },
    [IWEVGENIE  - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = IW_GENERIC_IE_MAX,
    },
    [IWEVMICHAELMICFAILURE  - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = sizeof(struct iw_michaelmicfailure),
    },
    [IWEVASSOCREQIE - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = IW_GENERIC_IE_MAX,
    },
    [IWEVASSOCRESPIE    - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = IW_GENERIC_IE_MAX,
    },
    [IWEVPMKIDCAND  - IWEVFIRST] = {
        .header_type    = IW_HEADER_TYPE_POINT,
        .token_size = 1,
        .max_tokens = sizeof(struct iw_pmkid_cand),
    },

};

static const unsigned 
standard_event_num = sizeof(standard_event)/sizeof(standard_event[0]);



/* Size (in bytes) of various events */
static const int 
priv_type_size[] = 
{
    0,                      /* IW_PRIV_TYPE_NONE */
    1,                      /* IW_PRIV_TYPE_BYTE */
    1,                      /* IW_PRIV_TYPE_CHAR */
    0,                      /* Not defined */
    sizeof(__u32),          /* IW_PRIV_TYPE_INT */
    sizeof(struct iw_freq), /* IW_PRIV_TYPE_FLOAT */
    sizeof(struct sockaddr),/* IW_PRIV_TYPE_ADDR */
    0,                      /* Not defined */
};
/************************** IOCTL SUPPORT **************************/
/*
 * The original user space API to configure all those Wireless Extensions
 * is through IOCTLs.
 * In there, we check if we need to call the new driver API (iw_handler)
 * or just call the driver ioctl handler.
 

  *  @iwp -> out, iwp points to &(wrq.u.data) of ioctl_standard_call(arg1,wrq, ...)
*/
/* ---------------------------------------------------------------- */
static int 
ioctl_standard_iw_point(struct iw_point *iwp,
                        unsigned int cmd,
                        const struct iw_ioctl_description *descr,
                        iw_handler handler,
                        moal_private * priv,
                        struct iw_request_info *info)
{
    int err, extra_size, user_length = 0, essid_compat = 0;
    char *extra;

    /* Calculate space needed by arguments. Always allocate
     * for max space.
     */
    extra_size = descr->max_tokens * descr->token_size;

    /* Check need for ESSID compatibility for WE < 21 */
    switch (cmd) {
        case SIOCSIWESSID:
        case SIOCGIWESSID:
        case SIOCSIWNICKN:
        case SIOCGIWNICKN:
            if (iwp->length == descr->max_tokens + 1)
                essid_compat = 1;
            else if (IW_IS_SET(cmd) && (iwp->length != 0)) 
            {
                char essid[IW_ESSID_MAX_SIZE + 1];

                ewl_os_memcpy(essid, iwp->pointer,iwp->length*descr->token_size);

                if (essid[iwp->length - 1] == '\0')
                    essid_compat = 1;
            }
            break;
        default:
            break;
    }

    iwp->length -= essid_compat;

    /* Check what user space is giving us */
    if (IW_IS_SET(cmd)) {
        /* Check NULL pointer */
        if (!iwp->pointer && iwp->length != 0)
            return -EFAULT;
        /* Check if number of token fits within bounds */
        if (iwp->length > descr->max_tokens)
            return -E2BIG;
        if (iwp->length < descr->min_tokens)
            return -EINVAL;
    } else {
        /* Check NULL pointer */
        if (!iwp->pointer)
            return -EFAULT;
        /* Save user space buffer size for checking */
        user_length = iwp->length;

        /* Don't check if user_length > max to allow forward
         * compatibility. The test user_length < min is
         * implied by the test at the end.
         */

        /* Support for very large requests */
        if ((descr->flags & IW_DESCR_FLAG_NOMAX) &&
            (user_length > descr->max_tokens)) {
            /* Allow userspace to GET more than max so
             * we can support any size GET requests.
             * There is still a limit : -ENOMEM.
             */
            extra_size = user_length * descr->token_size;

            /* Note : user_length is originally a __u16,
             * and token_size is controlled by us,
             * so extra_size won't get negative and
             * won't overflow...
             */
        }
    }
    /* ewl_os_malloc() ensures NULL-termination for essid_compat. */
    extra = ewl_os_malloc(extra_size);
    if (!extra)
        return -ENOMEM;
        
    /* If it is a SET, get all the extra data in here */
    if (IW_IS_SET(cmd) && (iwp->length != 0)) 
        ewl_os_memcpy(extra,iwp->pointer, iwp->length*descr->token_size);
        
    err = handler(priv, info, (struct iw_point *) iwp, extra);
    iwp->length += essid_compat;

    /* If we have something to return to the user */
    if (!err && IW_IS_GET(cmd)) {
        /* Check if there is enough buffer up there */
        if (user_length < iwp->length) {
            err = -E2BIG;
            goto out;
        }

        ewl_os_memcpy(iwp->pointer, extra, iwp->length*descr->token_size);
    }
    
out:
    ewl_os_free(extra);
    return err;
}

/* ---------------------------------------------------------------- */
/*
 * Standard Wireless Handler : get iwpriv definitions
 * Export the driver private handler definition
 * They will be picked up by tools like iwpriv...
 */
static int 
iw_handler_get_private(moal_private  *priv,
                                  struct iw_request_info *info,
                                  union iwreq_data *wrqu,
                                  char *extra)
{
#if 0
    /* Check if the driver has something to export */
    if ((num_private_args == 0) || (woal_private_args == NULL))
        return -EOPNOTSUPP;

    /* Check if there is enough buffer up there */
    if (wrqu->data.length < num_private_args) {
        /* User space can't know in advance how large the buffer
         * needs to be. Give it a hint, so that we can support
         * any size buffer we want somewhat efficiently... */
        wrqu->data.length = num_private_args;
        return -E2BIG;
    }

    /* Set the number of available ioctls. */
    wrqu->data.length = num_private_args;

    /* Copy structure to the user buffer. */
    ewl_os_memcpy(extra, woal_private_args,
           sizeof(struct iw_priv_args) * wrqu->data.length);
#endif
    return 0;
	UNUSED(priv);UNUSED(info);UNUSED(wrqu);UNUSED(extra);
}


/*
 * Wrapper to call a standard Wireless Extension handler.
 * We do various checks and also take care of moving data between
 * user space and kernel space.
 */
static int 
ioctl_standard_call(moal_private *priv,
                    struct iwreq *iwr,
                    unsigned int cmd,
                    struct iw_request_info  *info,
                    iw_handler handler)
{
    const struct iw_ioctl_description * descr;
    int                 ret = -EINVAL;
    
    ENTER();

    /* Get the description of the IOCTL */
    if ((cmd - SIOCIWFIRST) >= standard_ioctl_num)
    {   
        return -EOPNOTSUPP;
    } 

    descr = &(standard_ioctl[cmd - SIOCIWFIRST]);

    /* Check if we have a pointer to user space data or not */
    if (descr->header_type != IW_HEADER_TYPE_POINT) 
    {
        /* No extra arguments. Trivial to handle */
        ret = handler(priv, info, (struct iw_point *)&(iwr->u), NULL);
    }
    else
    {
        ret = ioctl_standard_iw_point(&iwr->u.data, cmd, descr,
                          handler, priv, info);
    }
    LEAVE();

    return ret;
}




#ifdef EXT_WPA_SUPPLICANT

/*
    input
        @priv : target queue 
        @pbuf : data to be put into queue

    @return :
        0 : fail
        1 : ok
*/

static int 
ewl_queue_pbuf(void *priv, void *pbuf)
{
    ENTER();
    
    mlan_rx_q_t *queue = priv;
    
    mlan_rxq_item_t *new_item = ewl_os_malloc(sizeof(mlan_rxq_item_t));
    
    if (new_item == NULL) goto fail;
    
    new_item->pbuf    = pbuf;
    new_item->next    = NULL;                       
                        
    if (queue->first_item != NULL)
    {
        queue->first_item->next = new_item;
    }
    else
    {
        queue->first_item = new_item;
    }
    queue->last_item = new_item;
    
    LEAVE();        
    return 1;
    
fail:
    return 0 ;   
    LEAVE();            
}

/*
    @ param pbuf 
    @ return 
      0 -> queue is empty.
      1 ->       have items.
      
    *note
    must free item in *pubf after use (if itme is allocated by malloc()) 
*/
static int
ewl_dequeue_pbuf(mlan_rx_q_t *queue, void **pbuf)
{
    mlan_rxq_item_t *item;
    
    ENTER();
    
    item = queue->first_item;

    if(item != NULL)
        queue->first_item = queue->first_item->next;
    else
        queue->last_item = NULL;
        
    if(item != NULL)
    {
        *pbuf = item->pbuf;
        ewl_os_free(item);
        return 1;
    }else
        *pbuf = NULL;
    
    LEAVE();
    return 0;
}

/*    
    @ event --> output  
    
 * @return 1 if an item was successfully received from the queue,
 * otherwise 0.
*/
int 
ewl_wireless_recv_event(struct iw_event  **event)
{
    int ret=0;

    moal_private *priv = ewl_get_sta_handle();
    if(priv != NULL )
    {
        ret = ewl_dequeue_pbuf(&priv->event_q, (void*)event);
    }
    return ret;
}


/*
 * Main event dispatcher. Called from other parts and drivers.
 * Send the event on the appropriate channels.
 */
void 
ewl_wireless_send_event(moal_private * priv,
                   unsigned int cmd,
                   union iwreq_data *wrqu,
                   char *extra)
{  
    const struct iw_ioctl_description * descr = NULL;
    int extra_len = 0;
    struct iw_event  *event;    /* Mallocated whole event */
    int event_len;              /* Its size */
    int hdr_len;                /* Size of the event header */
    int wrqu_off = 0;           /* Offset in wrqu */
    /* Don't "optimise" the following variable, it will crash */
    unsigned    cmd_index;      /* *MUST* be unsigned */
    
    /* Get the description of the Event */
    if (cmd <= SIOCIWLAST) {
        cmd_index = cmd - SIOCIWFIRST;
        if (cmd_index < standard_ioctl_num)
            descr = &(standard_ioctl[cmd_index]);
    } else {
        cmd_index = cmd - IWEVFIRST;
        if (cmd_index < standard_event_num)
            descr = &(standard_event[cmd_index]);
    }   
    
    /* Don't accept unknown events */
    if (descr == NULL) {
        /* Note : we don't return an error to the driver, because
         * the driver would not know what to do about it. It can't
         * return an error to the user, because the event is not
         * initiated by a user request.
         * The best the driver could do is to log an error message.
         * We will do it ourselves instead...
         */
        PRINTM(MMSG,"(WE) : Invalid/Unknown Wireless Event (0x%04X)\n", cmd);
        return;
    }

    /* Check extra parameters and set extra_len */
    if (descr->header_type == IW_HEADER_TYPE_POINT) {
        /* Check if number of token fits within bounds */
        if (wrqu->data.length > descr->max_tokens) {
            PRINTM(MMSG,"(WE) : Wireless Event too big (%d)\n", wrqu->data.length);
            return;
        }
        if (wrqu->data.length < descr->min_tokens) {
            PRINTM(MMSG,"(WE) : Wireless Event too small (%d)\n", wrqu->data.length);
            return;
        }
        /* Calculate extra_len - extra is NULL for restricted events */
        if (extra != NULL)
            extra_len = wrqu->data.length * descr->token_size;
        /* Always at an offset in wrqu */
        wrqu_off = IW_EV_POINT_OFF; //-->4
    }

    /* Total length of the event */
    hdr_len = event_type_size[descr->header_type];
    event_len = hdr_len + extra_len;

    /* Create temporary buffer to hold the event */
    event = ewl_os_malloc(event_len);
    if (event == NULL)
        return;

    /* Fill event */
    event->len = event_len;
    event->cmd = cmd;
    ewl_os_memcpy(&event->u, ((char *) wrqu) + wrqu_off, hdr_len - IW_EV_LCP_LEN);
    if (extra)
        ewl_os_memcpy(((char *) event) + hdr_len, extra, extra_len);
        
    /* Send event via priv->event */
    ewl_queue_pbuf(&priv->event_q,(void*)event);

    return;     /* Always success, I guess ;-) */
}
#endif

/*------------------------------------------------------------------*/
/*
 * Initialise the struct stream_descr so that we can extract
 * individual events from the event stream.
 */
void
iw_init_event_stream(struct stream_descr *  stream, /* Stream of events */
             char *         data,
             int            len)
{
    /* Cleanup */
    ewl_os_memset((char *) stream, '\0', sizeof(struct stream_descr));

    /* Set things up */
    stream->current = data;
    stream->end = data + len;
}

/*------------------------------------------------------------------*/
/*
 * Extract the next event from the event stream.
 *
 * @ stream : Stream of events
 * @ iwe    : Extracted event
 */
int
iw_extract_event_stream(struct stream_descr *   stream, struct iw_event *iwe)
{
    const struct iw_ioctl_description * descr = NULL;
    int     event_type = 0;
    unsigned int    event_len = 1;      /* Invalid */
    char *  pointer;
    /* Don't "optimise" the following variable, it will crash */
    unsigned    cmd_index;      /* *MUST* be unsigned */

    /* Check for end of stream */
    if((stream->current + IW_EV_LCP_PK_LEN) > stream->end)
    return(0);

    /* Extract the event header (to get the event id).
    * Note : the event may be unaligned, therefore copy... */
    ewl_os_memcpy((char *) iwe, stream->current, IW_EV_LCP_PK_LEN);

    /* Check invalid events */
    if(iwe->len <= IW_EV_LCP_PK_LEN)
        return(-1);

    /* Get the type and length of that event */
    if(iwe->cmd <= SIOCIWLAST)
    {
        cmd_index = iwe->cmd - SIOCIWFIRST;
        if(cmd_index < standard_ioctl_num)
            descr = &(standard_ioctl[cmd_index]);
    }
    else
    {
        cmd_index = iwe->cmd - IWEVFIRST;
        if(cmd_index < standard_event_num)
            descr = &(standard_event[cmd_index]);
    }
    if(descr != NULL)
        event_type = descr->header_type;
    /* Unknown events -> event_type=0 => IW_EV_LCP_PK_LEN */
    event_len = event_type_size[event_type];

    /* Check if we know about this event */
    if(event_len <= IW_EV_LCP_PK_LEN)
    {
        /* Skip to next event */
        stream->current += iwe->len;
        return(2);
    }
    event_len -= IW_EV_LCP_PK_LEN;

    /* Set pointer on data */
    if(stream->value != NULL)
        pointer = stream->value;            /* Next value in event */
    else
        pointer = stream->current + IW_EV_LCP_PK_LEN;   /* First value in event */

    /* Copy the rest of the event (at least, fixed part) */
    if((pointer + event_len) > stream->end)
    {
        /* Go to next event */
        stream->current += iwe->len;
        return(-2);
    }
    /* Fixup for WE-19 and later : pointer no longer in the stream */
    /* Beware of alignement. Dest has local alignement, not packed */
    if(event_type == IW_HEADER_TYPE_POINT)
        ewl_os_memcpy((char *) iwe + IW_EV_LCP_LEN + IW_EV_POINT_OFF,pointer, event_len);
    else
        ewl_os_memcpy((char *) iwe + IW_EV_LCP_LEN, pointer, event_len);
    /* Skip event in the stream */
    pointer += event_len;

    /* Special processing for iw_point events */
    if(event_type == IW_HEADER_TYPE_POINT)
    {
        /* Check the length of the payload */
        unsigned int    extra_len = iwe->len - (event_len + IW_EV_LCP_PK_LEN);
        if(extra_len > 0)
        {
            /* Set pointer on variable part (warning : non aligned) */
            iwe->u.data.pointer = pointer;
            /* Check that we have a descriptor for the command */
            if(descr == NULL)
            /* Can't check payload -> unsafe... */
                iwe->u.data.pointer = NULL; /* Discard paylod */
            
            else
            {
                /* Those checks are actually pretty hard to trigger,
                * because of the checks done in the kernel... */
                unsigned int    token_len = iwe->u.data.length * descr->token_size;
                /* Ugly fixup for alignement issues.
                * If the kernel is 64 bits and userspace 32 bits,
                * we have an extra 4+4 bytes.
                * Fixing that in the kernel would break 64 bits userspace. */
                if((token_len != extra_len) && (extra_len >= 4))
                {
                    __u16       alt_dlen = *((__u16 *) pointer);
                    unsigned int    alt_token_len = alt_dlen * descr->token_size;
                    if((alt_token_len + 8) == extra_len)
                    {
                        /* Ok, let's redo everything */
                        pointer -= event_len;
                        pointer += 4;
                        /* Dest has local alignement, not packed */
                        ewl_os_memcpy((char *) iwe + IW_EV_LCP_LEN + IW_EV_POINT_OFF,
                         pointer, event_len);
                        pointer += event_len + 4;
                        iwe->u.data.pointer = pointer;
                        token_len = alt_token_len;
                    }
                }   
                /* Discard bogus events which advertise more tokens than
                * what they carry... */
                if(token_len > extra_len)
                    iwe->u.data.pointer = NULL; /* Discard paylod */
                /* Check that the advertised token size is not going to
                * produce buffer overflow to our caller... */
                if((iwe->u.data.length > descr->max_tokens)
                && !(descr->flags & IW_DESCR_FLAG_NOMAX))
                    iwe->u.data.pointer = NULL; /* Discard paylod */
                /* Same for underflows... */
                if(iwe->u.data.length < descr->min_tokens)
                    iwe->u.data.pointer = NULL; /* Discard paylod */    
            }
        }
        else
            /* No data */
            iwe->u.data.pointer = NULL;
        /* Go to next event */
        stream->current += iwe->len;
    }
    else
    {
        /* Ugly fixup for alignement issues.
        * If the kernel is 64 bits and userspace 32 bits,
        * we have an extra 4 bytes.
        * Fixing that in the kernel would break 64 bits userspace. */
        if((stream->value == NULL)
            && ((((iwe->len - IW_EV_LCP_PK_LEN) % event_len) == 4)
            || ((iwe->len == 12) && ((event_type == IW_HEADER_TYPE_UINT) ||
            (event_type == IW_HEADER_TYPE_QUAL))) ))
        {
            pointer -= event_len;
            pointer += 4;
            /* Beware of alignement. Dest has local alignement, not packed */
            ewl_os_memcpy((char *) iwe + IW_EV_LCP_LEN, pointer, event_len);
            pointer += event_len;
        }

        /* Is there more value in the event ? */
        if((pointer + event_len) <= (stream->current + iwe->len))
            /* Go to next value */
            stream->value = pointer;
        else
        {
            /* Go to next event */
            stream->value = NULL;
            stream->current += iwe->len;
        }
    }
    return(1);
}



/************************* MISC SUBROUTINES **************************/



/*------------------------------------------------------------------*/
/*
 * Max size in bytes of an private argument.
 */
int
iw_get_priv_size(int args)
{
    int num = args & IW_PRIV_SIZE_MASK;
    int type = (args & IW_PRIV_TYPE_MASK) >> 12;
    
    return(num * priv_type_size[type]);
}

/*
    @ handle : pointer to wifi device driver (std mode)
    @ wifi wext api interface
    @wrq points to instance of struct iwreq * 
*/
int 
ewl_wext_driver(int handle, void *piwreq, u32 cmd)
{
    moal_private *priv = (moal_private *)handle;
    struct iw_request_info info = { .cmd = cmd, .flags = 0 };
    struct iw_handler_def *def=priv->wireless_handler;
    iw_handler  handler;
    u32         index;      /* *MUST* be unsigned */
    int ret = -EOPNOTSUPP;
    struct iwreq *wrq = piwreq;

    ENTER();
    
//    if(cmd == SIOCGIWPRIV) /* get private function args */
//        return ioctl_standard_call(priv,wrq,cmd,&info,iw_handler_get_private);
            
    /* find iw_handler*/
    index = cmd - SIOCIWFIRST;
    
    if(index < def->num_standard)
    {   
        handler = def->standard[index];
        if(handler != NULL)
        {
            ret=ioctl_standard_call(priv,wrq,cmd,&info,handler);
        }
        else
            return ret; 
    }
    LEAVE();
    return ret;
}
