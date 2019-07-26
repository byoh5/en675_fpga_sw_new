#ifndef _MOAL_MAIN_H
#define _MOAL_MAIN_H

#include <ewl.h>
#include <ewl_os.h>

#include "mlan.h"
#include "wireless.h"
#include "if.h"
#include "errno.h"
#include "moal_util.h"


#ifndef __BOOLEAN__
#define __BOOLEAN__
typedef unsigned char BOOLEAN;       /** Define BOOLEAN */
#endif
#define PBUF_HEAD_ROOM			16
//#define EXT_WPA_SUPPLICANT

#define MLAN_UNUSED_ARG(x) (void)x  

/** Driver version */
extern const char driver_version[];

#ifdef STA_SUPPORT
/** Driver mode STA bit */
#define DRV_MODE_STA       MBIT(0)
/** Maximum STA BSS */
#define MAX_STA_BSS        1
/** Default STA BSS */
#define DEF_STA_BSS        1
#endif
#ifdef UAP_SUPPORT
/** Driver mode uAP bit */
#define DRV_MODE_UAP       MBIT(1)
/** Maximum uAP BSS */
#define MAX_UAP_BSS        1
/** Default uAP BSS */
#define DEF_UAP_BSS        1
#endif
#if defined(WIFI_DIRECT_SUPPORT)
/** Driver mode WIFIDIRECT bit */
#define DRV_MODE_WIFIDIRECT       MBIT(2)
/** Maximum WIFIDIRECT BSS */
#define MAX_WIFIDIRECT_BSS        1
/** Default WIFIDIRECT BSS */
#define DEF_WIFIDIRECT_BSS        1
#endif /* WIFI_DIRECT_SUPPORT && V14_FEATURE */

#define SIOCDEVPRIVATE  0x89F0      /* to 89FF, defined in linux kernel originally */

typedef struct _moal_handle moal_handle;   /** Handle data structure for MOAL  */

/** Hardware status codes */
typedef enum _MOAL_HARDWARE_STATUS
{
    HardwareStatusReady,
    HardwareStatusInitializing,
    HardwareStatusFwReady,
    HardwareStatusReset,
    HardwareStatusClosing,
    HardwareStatusNotReady
} MOAL_HARDWARE_STATUS;

/** moal_wait_option */
enum
{
    MOAL_NO_WAIT,
    MOAL_IOCTL_WAIT,
    MOAL_CMD_WAIT,
    MOAL_PROC_WAIT,
    MOAL_WSTATS_WAIT
};

/** moal_main_state */
enum
{
    MOAL_STATE_IDLE,
    MOAL_RECV_INT,
    MOAL_ENTER_WORK_QUEUE,
    MOAL_START_MAIN_PROCESS,
    MOAL_END_MAIN_PROCESS
};

/** HostCmd_Header */
typedef struct _HostCmd_Header
{
    t_u16 command;  /** Command */
    t_u16 size;     /** Size */
} HostCmd_Header;

typedef struct _moal_drv_mode
{
    t_u16 drv_mode;                       /** driver mode */
    t_u16 intf_num;                       /** total number of interfaces */
    mlan_bss_attr *bss_attr;              /** attribute of bss */
    char *fw_name;                        /** name of firmware image */
} moal_drv_mode;

typedef struct {
    struct pbuf *buf[WLIF_QUEUELEN];
    u8 first;
    u8 last;
    u8 size;
} pqueue_t;

#define PQUEUE_EMPTY(q) (q.size == 0)
#define PQUEUE_FULL(q) (q.size == WLIF_QUEUELEN)
#define PQUEUE_FIRST(q) (q.buf[q.first])
#define PQUEUE_DEQUEUE(q)                                   \
        ({                                                  \
                if(q.size == 1)                             \
                {                                           \
                                                            \
                    q.first = -1;                           \
                    q.last  = -1;                           \
                }                                           \
                else                                        \
                {                                           \
                    q.first = (q.first + 1) % WLIF_QUEUELEN;\
                }                                           \
                q.size-- ;                                  \
        })
        
#define PQUEUE_ENQUEUE(q, p)                                \
        ({                                                  \
                if(q.size == 0)                             \
                {                                           \
                    q.first = 0;                            \
                    q.last  = 0;                            \
                }                                           \
                else                                        \
                    q.last = (q.last + 1) % WLIF_QUEUELEN;  \
                q.buf[q.last] = p;                          \
                q.size++;                                   \
        }) 
/* Modes of operation */

typedef struct _moal_private moal_private;          /** Private structure for MOAL */


typedef struct 
{
    void              *pbuf;
    struct mlan_rx_q *next;    
}mlan_rxq_item_t;

typedef struct mlan_rx_q
{
    mlan_rxq_item_t *first_item;
    mlan_rxq_item_t *last_item;
} mlan_rx_q_t;   

/*This struct should be hide from user */
/** Private structure for MOAL */
struct _moal_private
{
    struct netif *netif;
    void *phandle;
    u32 num_tx_timeout;                     /* Tx timeout count        */       
    u8 bss_index;                           /* BSS index               */       
    u8 bss_type;                            /* BSS type                */        
    u8 bss_role;                            /* BSS role                */        
    u8 current_addr[ETH_ALEN];              /* MAC address information */        
    u8 media_connected;                     /* Media connection status */
#ifdef UAP_SUPPORT
    u8 bss_started;                         /** uAP started or not     */ 
#endif                                                   

#ifdef EXT_WPA_SUPPLICANT
    mlan_rx_q_t event_q;                    /* event queue to communicate with supplicant */
#endif

    u16 ioctl_wait_q_woken;                 /* IOCTL wait queue token */                                     
    u16 cmd_wait_q_woken;                   /* Cmd wait queue token */                            

#ifdef STA_SUPPORT        
    u8 nick_name[16];                       /* Nickname */       
    u8 is_adhoc_link_sensed;                /* AdHoc link sensed flag */       
    u8 scan_block_flag;                     
    struct iw_statistics w_stats;           /** Block scan flag */
    u16 current_key_index;         
    u16 rate_index;                         /** Rate index */    
    u32 async_sem;               
    u8  scan_pending_on_block;              /** Scan pending on blocked flag */        
    u8  report_scan_result;                 /** Report scan result */        
    u8  wpa_version;                        /** wpa_version */           
    u8  key_mgmt;                           /** key mgmt    */
#endif //STA_SUPPORT                        /* STA_SUPPORT  */

    void *wireless_handler;    
    /* callback function when wifi (both of sta mode and s/w mode)
       is connected and disconnected                                          */
    void (*connected_cb)(BYTE *buf, int buf_len);
    void (*dis_connected_cb)(BYTE *buf, int buf_len); 
    /* support Lwip, rx queue */
    pqueue_t pqueue; 
};


/** Handle data structure for MOAL */
struct _moal_handle
{    
    t_void *pmlan_adapter;                /** MLAN adapter structure */        
    moal_private *priv[MLAN_MAX_BSS_NUM]; /** Private pointer */        
    t_u8 priv_num;                        /** Priv number */        
    moal_drv_mode drv_mode;              /** Bss attr */        
    t_u8 set_mac_addr;                    /**  MAC address */
    t_u8 mac_addr[ETH_ALEN];              
                                                                                                         
    const struct firmware *firmware;      /** Firmware */        
    MOAL_HARDWARE_STATUS hardware_status; /** STATUS variables */        

    BOOLEAN surprise_removed;             /** POWER MANAGEMENT AND PnP SUPPORT */        
    t_u32 fw_release_number;              /** Firmware release number */       
    t_u16 init_wait_q_woken;              /** Init wait queue token */       
                                          
    t_void *card;                         /** Card pointer */        
    t_u32 ioctl_pending;                  /** IOCTL pending count in mlan */       
    t_u32 malloc_count;                   /** Malloc count */        
	t_u32 vmalloc_count;
    t_u32 lock_count;                     /** lock count */        
    t_u32 mbufalloc_count;                /** mlan buffer alloc count */
    
            
    t_u8 main_state;                      /** main state */    
};

/*
 * OS timer specific
 */
struct timer_handler
{
	TimerHandle_t id; /* timer id that we use. */
    void (*handler)(void *ctx);
    void *data;
};


/** Timer structure */
typedef struct _moal_drv_timer
{
    struct timer_handler ti;
    void (*timer_function) (void *context);/** Timer function */  
    void *function_context;                /** Timer function context */
    t_u32 time_period;                     /** Time period */       
    t_u32 timer_is_periodic;               /** Is timer periodic ? */
    t_u32 timer_is_canceled;               /** Is timer cancelled ? */
} moal_drv_timer, *pmoal_drv_timer;


/** 
 *  @brief Timer handler
 *  
 *  @param fcontext Timer context
 *
 *  @return     N/A
 */
static inline void
woal_timer_handler(void* fcontext)
{
    pmoal_drv_timer timer = (pmoal_drv_timer) fcontext;

    timer->timer_function(timer->function_context);
}

/** 
 *  @brief Initialize timer
 *  
 *  @param timer        Timer structure
 *  @param TimerFunction    Timer function
 *  @param FunctionContext  Timer function context
 *
 *  @return         N/A
 */
static inline void
woal_initialize_timer(pmoal_drv_timer timer,
                      void (*TimerFunction) (void *context),
                      void *FunctionContext)
{
    /* Then tell the proxy which function to call and what to pass it */
    
    timer->ti.handler = woal_timer_handler; /* timer call this function that call timer->timer_function. */
    timer->ti.data    = timer;
        
    timer->timer_function = TimerFunction;
    timer->function_context = FunctionContext;
    timer->timer_is_canceled = MTRUE;
    timer->time_period = 0;
    timer->timer_is_periodic = MFALSE;
}

/** 
 *  @brief Modify timer
 *  
 *  @param timer        Timer structure
 *  @param MillisecondPeriod    Time period in millisecond
 *
 *  @return         N/A
 */
static inline void
woal_mod_timer(pmoal_drv_timer timer, t_u32 MillisecondPeriod)
{
    timer->time_period = MillisecondPeriod;
    timer->timer_is_canceled = MFALSE;
#if 1
    timer->ti.id = ewl_os_timer_sched_timeout_cb(MillisecondPeriod,
                                           timer->timer_is_periodic,
                                           timer->ti.handler,
                                           timer->ti.data);
#else
    timer->ti.id = timer_sched_timeout_cb(MillisecondPeriod,
                                           timer->timer_is_periodic, 
                                           timer->ti.handler,
                                           timer->ti.data);

#endif
}

/** 
 *  @brief Cancel timer
 *  
 *  @param timer    Timer structure
 *
 *  @return     N/A
 */
static inline void
woal_cancel_timer(moal_drv_timer * timer)
{
    timer->timer_is_canceled = MTRUE;
    timer->time_period = 0;
    ewl_os_timer_cancel_timeout(timer->ti.id);
}


/** wait_queue structure */
typedef struct _wait_queue
{
#ifdef FREE_RTOS_SUPPORT    
    xQueueHandle wait;      /** Pointer to wait_queue_head */          
#endif                      
    t_u16 *condition;       /** Pointer to wait condition  */
    t_u32 start_time;       /** Start time                 */
    mlan_status status;     /** Status from MLAN           */
} wait_queue, *pwait_queue;


#define MOAL_TIMER_10S                10000 /** 10 seconds */ 
#define MOAL_TIMER_5S                 5000  /** 5 seconds  */  
#define MOAL_TIMER_1S                 1000  /** 1 second   */
#define REASSOC_TIMER_DEFAULT         500
#define MOAL_IOCTL_TIMEOUT            5000 /* 5 seconds*/

/** BSS number bit mask */
#define BSS_NUM_MASK        7

#ifdef FREE_RTOS_SUPPORT
/** Initialize semaphore */
#define MOAL_INIT_SEMAPHORE(x)              vSemaphoreCreateBinary(x)
/** Acquire semaphore and with blocking */
#define MOAL_ACQ_SEMAPHORE_BLOCK(x)         xSemaphoreTake(x,0xffff)
/** Release semaphore */
#define MOAL_REL_SEMAPHORE(x)               xSemaphoreGive(x)       
#else
/** Initialize semaphore */
#define MOAL_INIT_SEMAPHORE(x)          
/** Acquire semaphore and with blocking */
#define MOAL_ACQ_SEMAPHORE_BLOCK(x) 
/** Release semaphore */
#define MOAL_REL_SEMAPHORE(x)
#endif

/** 16 bits byte swap */
#define swbyte_16(x) \
((t_u16)((((t_u16)(x) & 0x00ffU) << 8) | \
         (((t_u16)(x) & 0xff00U) >> 8)))

/** 32 bits byte swap */
#define swbyte_32(x) \
((t_u32)((((t_u32)(x) & 0x000000ffUL) << 24) | \
         (((t_u32)(x) & 0x0000ff00UL) <<  8) | \
         (((t_u32)(x) & 0x00ff0000UL) >>  8) | \
         (((t_u32)(x) & 0xff000000UL) >> 24)))

/** 64 bits byte swap */
#define swbyte_64(x) \
((t_u64)((t_u64)(((t_u64)(x) & 0x00000000000000ffULL) << 56) | \
         (t_u64)(((t_u64)(x) & 0x000000000000ff00ULL) << 40) | \
         (t_u64)(((t_u64)(x) & 0x0000000000ff0000ULL) << 24) | \
         (t_u64)(((t_u64)(x) & 0x00000000ff000000ULL) <<  8) | \
         (t_u64)(((t_u64)(x) & 0x000000ff00000000ULL) >>  8) | \
         (t_u64)(((t_u64)(x) & 0x0000ff0000000000ULL) >> 24) | \
         (t_u64)(((t_u64)(x) & 0x00ff000000000000ULL) >> 40) | \
         (t_u64)(((t_u64)(x) & 0xff00000000000000ULL) >> 56) ))

#ifdef BIG_ENDIAN_SUPPORT
/** Convert from 16 bit little endian format to CPU format */
#define woal_le16_to_cpu(x) swbyte_16(x)
/** Convert from 32 bit little endian format to CPU format */
#define woal_le32_to_cpu(x) swbyte_32(x)
/** Convert from 64 bit little endian format to CPU format */
#define woal_le64_to_cpu(x) swbyte_64(x)
/** Convert to 16 bit little endian format from CPU format */
#define woal_cpu_to_le16(x) swbyte_16(x)
/** Convert to 32 bit little endian format from CPU format */
#define woal_cpu_to_le32(x) swbyte_32(x)
/** Convert to 64 bit little endian format from CPU format */
#define woal_cpu_to_le64(x) swbyte_64(x)
#else
/** Do nothing */
#define woal_le16_to_cpu(x) x
/** Do nothing */
#define woal_le32_to_cpu(x) x
/** Do nothing */
#define woal_le64_to_cpu(x) x
/** Do nothing */
#define woal_cpu_to_le16(x) x
/** Do nothing */
#define woal_cpu_to_le32(x) x
/** Do nothing */
#define woal_cpu_to_le64(x) x
#endif

extern int sdio_irq;
char *
woal_strsep(char **s, char delim, char esc);

mlan_status
woal_atoi(int *data, char *a);
int
woal_ascii2hex(t_u8 * d, char *s, t_u32 dlen);


mlan_status
woal_init_fw(moal_handle * handle);


/** Interrupt handler */
void woal_interrupt(moal_handle * handle);
/** Add card */
moal_handle *woal_add_card(void *card);
/** Allocate buffer */
pmlan_buffer woal_alloc_mlan_buffer(moal_handle * handle, int size);
/** Allocate IOCTL request buffer */
pmlan_ioctl_req woal_alloc_mlan_ioctl_req(int size);
/** Get private structure of a BSS by index */
moal_private *woal_bss_index_to_priv(moal_handle * handle, t_u8 bss_num);
/** associate */
mlan_status woal_bss_start(moal_private * priv, t_u8 wait_option,
                           mlan_ssid_bssid * ssid_bssid);
/** Free buffer */
void woal_free_mlan_buffer(moal_handle * handle, pmlan_buffer pmbuf);  

/** Request firmware information */
void woal_request_get_fw_info(moal_private * priv, t_u8 wait_option);

int
woal_set_deep_sleep(moal_private * priv, t_u8 wait_option, BOOLEAN bdeep_sleep,
                    t_u16 idletime);

mlan_status
woal_cancel_hs(moal_private * priv, t_u8 wait_option);  

mlan_status
woal_request_scan(moal_private * priv,
                  t_u8 wait_option, mlan_802_11_ssid * req_ssid);

mlan_status
woal_disconnect(moal_private * priv, t_u8 wait_option, t_u8 * mac);



/** Request IOCTL action */
mlan_status woal_request_ioctl(moal_private * priv, mlan_ioctl_req * req,
                               t_u8 wait_option);
int
woal_host_command(moal_private * priv, struct iwreq *wrq);                               
/** Set/get Host Sleep parameters */
mlan_status woal_set_get_hs_params(moal_private * priv, t_u16 action,
                                   t_u8 wait_option, mlan_ds_hs_cfg * hscfg);
                                   
#if defined(UAP_SUPPORT)
/** hostcmd ioctl for uap, wifidirect */
int woal_hostcmd_ioctl(moal_private *priv, struct ifreq *req);
#endif                               

mlan_status
woal_get_bss_info(moal_private * priv, t_u8 wait_option,
                  mlan_bss_info * bss_info);

int
woal_custom_ie_ioctl(moal_private *priv, struct ifreq *req);

int
woal_get_bss_type(moal_private *priv, struct ifreq *req);

int
woal_hard_start_xmit(struct pbuf* p , moal_private *priv);

void *
ewl_get_sta_handle(void);

mlan_status
woal_request_set_mac_address(moal_private * priv);


#endif /* _MOAL_MAIN_H */
