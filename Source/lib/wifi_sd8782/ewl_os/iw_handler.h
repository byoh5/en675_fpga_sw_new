#ifndef _IW_HANDLER_H
#define _IW_HANDLER_H

/************************* INLINE FUNTIONS *************************/
/*
 * Function that are so simple that it's more efficient inlining them
 */

static inline int iwe_stream_lcp_len(struct iw_request_info *info)
{
    MLAN_UNUSED_ARG(info);
	return IW_EV_LCP_LEN;
}

static inline int iwe_stream_point_len(struct iw_request_info *info)
{
    MLAN_UNUSED_ARG(info);
	return IW_EV_POINT_LEN;
}

static inline int iwe_stream_event_len_adjust(struct iw_request_info *info,
					      int event_len)
{
    MLAN_UNUSED_ARG(info);
	return event_len;
}

/*------------------------------------------------------------------*/
/*
 * Wrapper to add an Wireless Event to a stream of events.
 */
static inline char *
iwe_stream_add_event(struct iw_request_info *info, char *stream, char *ends,
		     struct iw_event *iwe, int event_len)
{
	int lcp_len = iwe_stream_lcp_len(info);
	event_len = iwe_stream_event_len_adjust(info, event_len);

	/* Check if it's possible */
	if((stream + event_len) < ends) {
		iwe->len = event_len;
		/* Beware of alignement issues on 64 bits */
		ewl_os_memcpy(stream, (char *) iwe, IW_EV_LCP_PK_LEN);
		ewl_os_memcpy(stream + lcp_len, &iwe->u,
		       event_len - lcp_len);
		stream += event_len;
	}
	return stream;
}

/*------------------------------------------------------------------*/
/*
 * Wrapper to add an short Wireless Event containing a pointer to a
 * stream of events.
 */
static inline char *
iwe_stream_add_point(struct iw_request_info *info, char *stream, char *ends,
		     struct iw_event *iwe, char *extra)
{
	int event_len = iwe_stream_point_len(info) + iwe->u.data.length;
	int point_len = iwe_stream_point_len(info);
	int lcp_len   = iwe_stream_lcp_len(info);

	/* Check if it's possible */
	if((stream + event_len) < ends) {
		iwe->len = event_len;
		ewl_os_memcpy(stream, (char *) iwe, IW_EV_LCP_PK_LEN);
		ewl_os_memcpy(stream + lcp_len,
		       ((char *) &iwe->u) + IW_EV_POINT_OFF,
		       IW_EV_POINT_PK_LEN - IW_EV_LCP_PK_LEN);
		ewl_os_memcpy(stream + point_len, extra, iwe->u.data.length);
		stream += event_len;
	}
	return stream;
}

/*------------------------------------------------------------------*/
/*
 * Wrapper to add a value to a Wireless Event in a stream of events.
 * Be careful, this one is tricky to use properly :
 * At the first run, you need to have (value = event + IW_EV_LCP_LEN).
 */
static inline char *
iwe_stream_add_value(struct iw_request_info *info, char *event, char *value,
		     char *ends, struct iw_event *iwe, int event_len)
{
	int lcp_len = iwe_stream_lcp_len(info);

	/* Don't duplicate LCP */
	event_len -= IW_EV_LCP_LEN;

	/* Check if it's possible */
	if((value + event_len) < ends) {
		/* Add new value */
		ewl_os_memcpy(value, &iwe->u, event_len);
		value += event_len;
		/* Patch LCP */
		iwe->len = value - event;
		ewl_os_memcpy(event, (char *) iwe, lcp_len);
	}
	return value;
}

/*
 * This define all the handler that the driver export.
 * As you need only one per driver type, please use a static const
 * shared by all driver instances... Same for the members...
 * This will be linked from net_device in <linux/netdevice.h>
 */
struct iw_handler_def {
    /* Array of handlers for standard ioctls
     * We will call dev->wireless_handlers->standard[ioctl - SIOCIWFIRST]
     */
    iw_handler *      standard;
    /* Number of handlers defined (more precisely, index of the
     * last defined handler + 1) */
    u16                   num_standard;
};

#endif	/* _IW_HANDLER_H */
