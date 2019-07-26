/** @file moal_shim.c
  *
  * @brief This file contains the callback functions registered to MLAN
  *
  * Copyright (C) 2008-2010, Marvell International Ltd. 
  * 
  * This software file (the "File") is distributed by Marvell International 
  * Ltd. under the terms of the GNU General Public License Version 2, June 1991 
  * (the "License").  You may use, redistribute and/or modify this File in 
  * accordance with the terms and conditions of the License, a copy of which 
  * is available by writing to the Free Software Foundation, Inc.,
  * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA or on the
  * worldwide web at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
  *
  * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE 
  * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE 
  * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about 
  * this warranty disclaimer.
  *
  */

/********************************************************
Change log:
    10/21/2008: initial version
********************************************************/


#include <ewl_os.h>
#include <lwip/pbuf.h>
#include <ewl_pkt_header.h>

#include "wireless.h"
#include "moal_main.h"
#include "moal_sdio.h"
#include "moal_util.h"
#include "moal_wext.h"
#include "stdio.h"

/********************************************************
		Local Variables
********************************************************/
/** moal_lock */
typedef struct _moal_lock
{      
	//spinlock_t lock;      /** Lock */
	unsigned long flags;    /** Flags */
} moal_lock;

u32 malloc_count=0;

/********************************************************
		Global Variables
********************************************************/

/********************************************************
		Local Functions
********************************************************/

u32 get_malloc_count(void)
{
    return malloc_count;
}

/********************************************************
		Global Functions
********************************************************/
/** 
 *  @brief Alloc a buffer 
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param size 	The size of the buffer to be allocated
 *  @param flag 	The type of the buffer to be allocated
 *  @param ppbuf	Pointer to a buffer location to store buffer pointer allocated
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_malloc(IN t_void * pmoal_handle, IN t_u32 size, IN t_u32 flag, OUT t_u8 ** ppbuf)
{
    moal_handle *handle = (moal_handle *) pmoal_handle;

    MLAN_UNUSED_ARG(flag);
    
    if (!(*ppbuf = ewl_os_malloc(size))) {
      PRINTM(MERROR, "%s: allocate  buffer %d failed!\n", __FUNCTION__,
             (int) size);
      return MLAN_STATUS_FAILURE;
    }
    handle->malloc_count++;
    malloc_count++;
    
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Free a buffer 
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pbuf		Pointer to the buffer to be freed
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_mfree(IN t_void * pmoal_handle, IN t_u8 * pbuf)
{
    moal_handle *handle = (moal_handle *) pmoal_handle;
    
    if (!pbuf)
        return MLAN_STATUS_FAILURE;
    ewl_os_free(pbuf);
    handle->malloc_count--;
    malloc_count--;
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Alloc a vitual-address-continuous buffer
 *
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param size     The size of the buffer to be allocated
 *  @param ppbuf    Pointer to a buffer location to store buffer pointer allocated
 *
 *  @return         MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_vmalloc(IN t_void * pmoal_handle, IN t_u32 size, OUT t_u8 ** ppbuf)
{
  moal_handle *handle = (moal_handle *) pmoal_handle;

  *ppbuf = ewl_os_malloc(size);
  if (*ppbuf == NULL) {
    PRINTM(MERROR, "%s: vmalloc (%d bytes) failed!", __func__,
           (int)size);
    return MLAN_STATUS_FAILURE;
  }
  handle->vmalloc_count++;

  return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Free a buffer allocated by vmalloc
 *
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pbuf     Pointer to the buffer to be freed
 *
 *  @return         MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_vfree(IN t_void * pmoal_handle, IN t_u8 * pbuf)
{
  moal_handle *handle = (moal_handle *) pmoal_handle;

  if (!pbuf)
    return MLAN_STATUS_FAILURE;
  ewl_os_free(pbuf);
  handle->vmalloc_count--;
  return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Fill memory with constant byte 
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pmem		Pointer to the memory area
 *  @param byte		A constant byte
 *  @param num		Number of bytes to fill
 *
 *  @return    		Pointer to the memory area
 */
t_void *
moal_memset(IN t_void * pmoal_handle,
            IN t_void * pmem, IN t_u8 byte, IN t_u32 num)
{
    t_void *p = pmem;

    MLAN_UNUSED_ARG(pmoal_handle);

    if (pmem && num)
        p = ewl_os_memset((char*)pmem, byte, num);
        //DmaMemSet((BYTE*)pmem, byte, num);
    return p;
}

/** 
 *  @brief Copy memory from one area to another
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pdest	Pointer to the dest memory
 *  @param psrc		Pointer to the src memory
 *  @param num		Number of bytes to move
 *
 *  @return    		Pointer to the dest memory
 */
t_void *
moal_memcpy(IN t_void * pmoal_handle,
            IN t_void * pdest, IN const t_void * psrc, IN t_u32 num)
{
    t_void *p = pdest;
    MLAN_UNUSED_ARG(pmoal_handle);
    if (pdest && psrc && num)
    p=ewl_os_memcpy(pdest, psrc, num);
    //DmaMemCpy((BYTE*)pdest,psrc,num, NULL);
    
    return p;
}

/** 
 *  @brief Move memory from one area to another
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pdest	Pointer to the dest memory
 *  @param psrc		Pointer to the src memory
 *  @param num		Number of bytes to move
 *
 *  @return    		Pointer to the dest memory
 */
t_void *
moal_memmove(IN t_void * pmoal_handle,
             IN t_void * pdest, IN const t_void * psrc, IN t_u32 num)
{
    t_void *p = pdest;
    MLAN_UNUSED_ARG(pmoal_handle);
    if (pdest && psrc && num)
    p = ewl_os_memmove(pdest, psrc, num);
    //DmaMemCpy((BYTE*)pdest,psrc,num, NULL);
    
    return p;
}


/** 
 *  @brief Compare two memory areas
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pmem1	Pointer to the first memory
 *  @param pmem2	Pointer to the second memory
 *  @param num		Number of bytes to compare
 *
 *  @return    		Compare result returns by memcmp
 */
t_s32
moal_memcmp(IN t_void * pmoal_handle,
            IN const t_void * pmem1, IN const t_void * pmem2, IN t_u32 num)
{
    t_s32 result;
    MLAN_UNUSED_ARG(pmoal_handle);
    result =ewl_os_memcmp(pmem1, pmem2, num);
    
    return result;
}

/** 
 *  @brief Retrieves the current system time
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param psec		Pointer to buf for the seconds of system time
 *  @param pusec 	Pointer to buf the micro seconds of system time
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_get_system_time(IN t_void * pmoal_handle,
                     OUT t_u32 * psec, OUT t_u32 * pusec)
{
    //struct timeval t;
    t_u32 ms = ewl_os_timer_get_ms();
    MLAN_UNUSED_ARG(pmoal_handle);
    *psec  = ms/1000;
    *pusec = (ms%1000)*1000;
    
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Initializes the timer
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pptimer	Pointer to the timer
 *  @param callback 	Pointer to callback function
 *  @param pcontext 	Pointer to context
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE 
 */
mlan_status
moal_init_timer(IN t_void * pmoal_handle,
                OUT t_void ** pptimer,
                IN t_void(*callback) (t_void * pcontext), IN t_void * pcontext)
{
    moal_drv_timer *timer = NULL;
    MLAN_UNUSED_ARG(pmoal_handle);
    if (!(timer = (moal_drv_timer *) ewl_os_malloc(sizeof(moal_drv_timer))))
        return MLAN_STATUS_FAILURE;
      
    woal_initialize_timer(timer, callback, pcontext);
    *pptimer = (t_void *) timer;
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Free the timer
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param ptimer	Pointer to the timer
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_free_timer(IN t_void * pmoal_handle, IN t_void * ptimer)
{
    moal_drv_timer *timer = (moal_drv_timer *) ptimer;
    MLAN_UNUSED_ARG(pmoal_handle);
    if (timer) {
        if ((timer->timer_is_canceled == MFALSE) && timer->time_period) {
            PRINTM(MWARN, "mlan try to free timer without stop timer!\n");
            woal_cancel_timer(timer);
        }
        ewl_os_free(timer);
    }

    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Start the timer
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param ptimer	Pointer to the timer
 *  @param periodic     Periodic timer
 *  @param msec		Timer value in milliseconds
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_start_timer(IN t_void * pmoal_handle,
                 IN t_void * ptimer, IN t_u8 periodic, IN t_u32 msec)
{

    MLAN_UNUSED_ARG(pmoal_handle);
    if (!ptimer)
        return MLAN_STATUS_FAILURE;
    ((moal_drv_timer *) ptimer)->timer_is_periodic = periodic;
    woal_mod_timer((moal_drv_timer *) ptimer, msec);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Stop the timer
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param ptimer	Pointer to the timer
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_stop_timer(IN t_void * pmoal_handle, IN t_void * ptimer)
{
    MLAN_UNUSED_ARG(pmoal_handle);
    if (!ptimer)
        return MLAN_STATUS_FAILURE;
    woal_cancel_timer((moal_drv_timer *) ptimer);
    return MLAN_STATUS_SUCCESS;
}


/** 
 *  @brief Initializes the lock
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pplock	Pointer to the lock
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE 
 */
mlan_status
moal_init_lock(IN t_void * pmoal_handle, OUT t_void ** pplock)
{
	moal_handle *handle = (moal_handle *) pmoal_handle;

	t_u32 *mlock = NULL;

	MLAN_UNUSED_ARG(pmoal_handle);
	if (!(mlock = (t_u32 *) ewl_os_malloc(sizeof(t_u32))))
        return MLAN_STATUS_FAILURE;
	*mlock = 0;
	*pplock = (t_void *) mlock;
	
	handle->lock_count++;
	
	return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Free the lock
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param plock	Lock
 *
 *  @return    		MLAN_STATUS_SUCCESS
 */
mlan_status
moal_free_lock(IN t_void * pmoal_handle, IN t_void * plock)
{
    moal_handle *handle = (moal_handle *) pmoal_handle;
    t_u32 *mlock = plock;

    MLAN_UNUSED_ARG(pmoal_handle);
    if (mlock) {
        ewl_os_free(mlock);
        handle->lock_count--;
    }

  return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief Request a spin lock
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param plock	Pointer to the lock
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_spin_lock(IN t_void * pmoal_handle, IN t_void * plock)
{
#if 1
    t_u32 *mlock = plock;
    
    MLAN_UNUSED_ARG(pmoal_handle);
    if (mlock) {
        ewl_os_disable_int();
        return MLAN_STATUS_SUCCESS;
    } 
    else {
        return MLAN_STATUS_FAILURE;
    }
#else    
     return MLAN_STATUS_SUCCESS;    
#endif
}

/** 
 *  @brief Request a spin_unlock
 *     
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param plock	Pointer to the lock
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_spin_unlock(IN t_void * pmoal_handle, IN t_void * plock)
{
#if 1
    t_u32 *mlock = (t_u32 *) plock;
    MLAN_UNUSED_ARG(pmoal_handle);
    if (mlock) {
        ewl_os_restore_int();  	
        return MLAN_STATUS_SUCCESS;
    } else {
        return MLAN_STATUS_FAILURE;
    }
#else    
    return MLAN_STATUS_SUCCESS;
#endif
}

/** 
 *  @brief This function reads one block of firmware data from MOAL
 *
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param offset       Offset from where the data will be copied
 *  @param len          Length to be copied
 *  @param pbuf         Buffer where the data will be copied
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_get_fw_data(IN t_void * pmoal_handle,
                 IN t_u32 offset, IN t_u32 len, OUT t_u8 * pbuf)
{
//  moal_handle *handle = (moal_handle *) pmoal_handle;
//  
//  if (!pbuf || !len)
//      return MLAN_STATUS_FAILURE;
//  
//  if (offset + len > handle->firmware->size)
//      return MLAN_STATUS_FAILURE;
//  
//  memcpy(pbuf, handle->firmware->data + offset, len);
    MLAN_UNUSED_ARG(pmoal_handle);
    MLAN_UNUSED_ARG(offset);
    MLAN_UNUSED_ARG(len);
    MLAN_UNUSED_ARG(pbuf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function is called when MLAN completes the initialization firmware.
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param status	The status code for mlan_init_fw request
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_init_fw_complete(IN t_void * pmoal_handle, IN mlan_status status)
{
	moal_handle *handle = (moal_handle *) pmoal_handle;
	ENTER();
	
	if (status == MLAN_STATUS_SUCCESS)
		handle->hardware_status = HardwareStatusReady;
	handle->init_wait_q_woken = MTRUE;
	LEAVE();
	return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function is called when MLAN shutdown firmware is completed.
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param status	The status code for mlan_shutdown request
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_shutdown_fw_complete(IN t_void * pmoal_handle, IN mlan_status status)
{
	moal_handle *handle = (moal_handle *) pmoal_handle;
	MLAN_UNUSED_ARG(status);
	ENTER();
	handle->hardware_status = HardwareStatusNotReady;
	handle->init_wait_q_woken = MTRUE;
//	wake_up_interruptible(&handle->init_wait_q);
	LEAVE();
  return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function is called when an MLAN IOCTL is completed.
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pioctl_req	pointer to structure mlan_ioctl_req 
 *  @param status	The status code for mlan_ioctl request
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_ioctl_complete(IN t_void * pmoal_handle,
                    IN pmlan_ioctl_req pioctl_req, IN mlan_status status)
{
	moal_handle *handle = (moal_handle *) pmoal_handle;
	moal_private *priv = NULL;
	wait_queue *wait;
	
	ENTER();
	
	priv = woal_bss_index_to_priv(handle, pioctl_req->bss_index);
	if (priv == NULL) {
		PRINTM(MERROR, "%s: priv is null\n", __FUNCTION__);
		ewl_os_free(pioctl_req);
		goto done;
	}
	
	wait = (wait_queue *) pioctl_req->reserved_1;
	PRINTM(MIOCTL,
	     "IOCTL completed: %p id=0x%lx sub_id=0x%lx, action=%d,  status=%d, status_code=0x%x\n",
	     pioctl_req, pioctl_req->req_id, (*(t_u32 *) pioctl_req->pbuf),
	     (int) pioctl_req->action, status, pioctl_req->status_code);

	if (wait) 
	{
		*wait->condition = MTRUE;
		wait->status = status;

		if ((status != MLAN_STATUS_SUCCESS) &&
			(pioctl_req->status_code == MLAN_ERROR_CMD_TIMEOUT)) {
			PRINTM(MERROR, "IOCTL: command timeout\n");
		} 
		else 
		{
			
#ifdef FREE_RTOS_SUPPORT			
			xQueueSend(wait->wait, (void *)wait->condition, 0);
#endif			
		}
	}
	else 
	{
#ifdef STA_SUPPORT
        //if ((status == MLAN_STATUS_SUCCESS) &&
        //(pioctl_req->action == MLAN_ACT_GET))
        //woal_process_ioctl_resp(priv, pioctl_req);
#endif
        if (status != MLAN_STATUS_SUCCESS)
        {
	    	PRINTM(MERROR,
	             "IOCTL failed: id=0x%lx, action=%d, status_code=0x%x\n",
	             pioctl_req->req_id, (int) pioctl_req->action,
	             pioctl_req->status_code);
		}
		ewl_os_free(pioctl_req);
	}
done:
	LEAVE();
	return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function allocates mlan_buffer.
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param size		allocation size requested 
 *  @param pmbuf	pointer to pointer to the allocated buffer 
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_alloc_mlan_buffer(IN t_void * pmoal_handle,
                       IN t_u32 size, OUT pmlan_buffer * pmbuf)
{
	if (NULL ==
		(*pmbuf = woal_alloc_mlan_buffer((moal_handle *) pmoal_handle, size)))
		return MLAN_STATUS_FAILURE;
	return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function frees mlan_buffer.
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pmbuf	pointer to buffer to be freed
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_free_mlan_buffer(IN t_void * pmoal_handle, IN pmlan_buffer pmbuf)
{
    if (!pmbuf)
        return MLAN_STATUS_FAILURE;
    woal_free_mlan_buffer((moal_handle *) pmoal_handle, pmbuf);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function is called when MLAN complete send data packet.
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pmbuf	Pointer to the mlan buffer structure
 *  @param status	The status code for mlan_send_packet request
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */


mlan_status
moal_send_packet_complete(IN t_void * pmoal_handle,
                          IN pmlan_buffer pmbuf, IN mlan_status status)
{
	moal_private *priv = NULL;
	struct pbuf* p;

    MLAN_UNUSED_ARG(pmoal_handle);
    MLAN_UNUSED_ARG(status);
	
	ENTER();
	if (pmbuf) 
	{
		priv = woal_bss_index_to_priv(pmoal_handle, pmbuf->bss_index);
		p = (struct pbuf *) pmbuf->pdesc;
		
		if (p)
		{
//			printf("FREE: p(0x%08X) - %s\n", p, __func__);
			pbuf_free(p);
        }
	}

	LEAVE();

	return MLAN_STATUS_SUCCESS;
}



/** 
 *  @brief This function write a command/data packet to card.
 *         This function blocks the call until it finishes
 *
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pmbuf	Pointer to the mlan buffer structure
 *  @param port 	Port number for sent
 *  @param timeout 	Timeout value in milliseconds (if 0 the wait is forever)
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_write_data_sync(IN t_void * pmoal_handle,
                     IN pmlan_buffer pmbuf, IN t_u32 port, IN t_u32 timeout)
{
#ifdef SDIO_SUSPEND_RESUME
    moal_handle *handle = (moal_handle *) pmoal_handle;
    if (handle->is_suspended == MTRUE) {
        PRINTM(MERROR, "write_data_sync is not allowed while suspended\n");
        return MLAN_STATUS_FAILURE;
    }
#endif
    
    return woal_write_data_sync((moal_handle *) pmoal_handle, pmbuf, port,
                                timeout);
    return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function read data packet/event/command from card.
 *         This function blocks the call until it finish
 *
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pmbuf	Pointer to the mlan buffer structure
 *  @param port 	Port number for read
 *  @param timeout 	Timeout value in milliseconds (if 0 the wait is forever)
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_read_data_sync(IN t_void * pmoal_handle,
                    IN OUT pmlan_buffer pmbuf, IN t_u32 port, IN t_u32 timeout)
{
    return woal_read_data_sync((moal_handle *) pmoal_handle, pmbuf, port,
                               timeout);
  return MLAN_STATUS_SUCCESS;                               
}

/** 
 *  @brief This function writes data into card register.
 *
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param reg          register offset
 *  @param data         value
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_write_reg(IN t_void * pmoal_handle, IN t_u32 reg, IN t_u32 data)
{
	return woal_write_reg((moal_handle *) pmoal_handle, reg, data);
}

/** 
 *  @brief This function reads data from card register.
 *
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param reg          register offset
 *  @param data         value
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status
moal_read_reg(IN t_void * pmoal_handle, IN t_u32 reg, OUT t_u32 * data)
{	
	return woal_read_reg((moal_handle *) pmoal_handle, reg, data);
}

/** 
 *  @brief Delay function
 *   
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param delay  delay in micro-second
 *
 *  @return    	  None
 */
t_void
moal_udelay(IN t_void * pmoal_handle, IN t_u32 delay)
{
    MLAN_UNUSED_ARG(pmoal_handle);
	ewl_os_wait_us(delay);
}

/** 
 *  @brief This function uploads the packet to the network stack
 *
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pmbuf	Pointer to the mlan buffer structure
 *
 *  @return    		MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
  
mlan_status
moal_recv_packet(IN t_void * pmoal_handle, IN pmlan_buffer pmbuf)
{
	mlan_status status = MLAN_STATUS_SUCCESS;
	moal_private *priv = NULL;
	struct pbuf *p;
	ENTER();
	if (pmbuf) {
	    priv = woal_bss_index_to_priv(pmoal_handle, pmbuf->bss_index);
        p = (struct pbuf *)pmbuf->pdesc;
	    if (priv) {
	        if (p) {
                pbuf_header(p, -pmbuf->data_offset);
                pbuf_realloc(p, pmbuf->data_len);
#if 1
                if (pmbuf->data_offset < MLAN_HEADER_SIZE) {
					struct pbuf *q = pbuf_alloc(PBUF_RAW, pmbuf->data_len + MLAN_HEADER_SIZE, PBUF_RAM);
					if (q == NULL) {
						printf("%s(%d) pbuf NULL! alloc Fail!(%dbyte)\n", __func__, __LINE__, pmbuf->data_len + MLAN_HEADER_SIZE);
					} else {
						pbuf_header(q, -MLAN_HEADER_SIZE);
						pbuf_copy(q, p);
						pbuf_free(p);
						p = q;
					}
				}
#endif
				pmbuf->pdesc = NULL;
                pmbuf->pbuf = NULL;
                pmbuf->data_offset = pmbuf->data_len = 0;
				ewl_os_disable_int();
                if (!PQUEUE_FULL(priv->pqueue)) {
                    PQUEUE_ENQUEUE(priv->pqueue, p);
                    ewl_os_restore_int();
               	} else {
               		ewl_os_restore_int();
                    PRINTM(MERROR, "buffer pull !!!\n");
					printf("%s(%d) rx drop\n", __func__, __LINE__);
                    pbuf_free(p);
                }
	        } else {    
	        	status = MLAN_STATUS_FAILURE;	 
			} 
		}
	}
	LEAVE();
	return status;
}

/** 
 *  @brief This function handles event receive
 *
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param pmevent	Pointer to the mlan event structure
 *
 *  @return    		MLAN_STATUS_SUCCESS 
 */
mlan_status
moal_recv_event(IN t_void * pmoal_handle, IN pmlan_event pmevent)
{
    moal_private *priv = NULL;
#ifdef STA_SUPPORT
    //char event[64];
    union iwreq_data wrqu;
#endif
    ENTER();

   
    priv = woal_bss_index_to_priv(pmoal_handle, pmevent->bss_index);
    if (priv == NULL) {
        PRINTM(MERROR, "%s: priv is null\n", __FUNCTION__);
        goto done;
    }

    switch (pmevent->event_id) {

#ifdef STA_SUPPORT
    case MLAN_EVENT_ID_FW_ADHOC_LINK_SENSED:
        PRINTM(MMSG,"MLAN_EVENT_ID_FW_ADHOC_LINK_SENSED\n");
        break;

    case MLAN_EVENT_ID_FW_ADHOC_LINK_LOST:
        break;

    case MLAN_EVENT_ID_DRV_CONNECTED:
        if (pmevent->event_len == ETH_ALEN) {
		    ewl_os_memset(wrqu.ap_addr.sa_data, 0x00, ETH_ALEN);
		    ewl_os_memcpy(wrqu.ap_addr.sa_data, pmevent->event_buf, ETH_ALEN);
		    wrqu.ap_addr.sa_family = ARPHRD_ETHER;
#ifdef EXT_WPA_SUPPLICANT		    
		    ewl_wireless_send_event(priv, SIOCGIWAP, &wrqu, NULL);
#endif
		    PRINTM(MMSG,"MLAN_EVENT_ID_DRV_CONNECTED \n");    
		}
    	
        priv->media_connected = MTRUE;
        break;

    case MLAN_EVENT_ID_DRV_SCAN_REPORT:
        PRINTM(MMSG,"MLAN_EVENT_ID_DRV_SCAN_REPORT\n");
        if (priv->report_scan_result) {
            ewl_os_memset(&wrqu, 0, sizeof(union iwreq_data));         
           	ewl_wireless_send_event(priv, SIOCGIWSCAN, &wrqu, NULL);
            priv->report_scan_result = MFALSE;
        }
        if (priv->scan_pending_on_block == MTRUE) {
            priv->scan_pending_on_block = MFALSE;
            MOAL_REL_SEMAPHORE(priv->async_sem);
        }
        break;

    case MLAN_EVENT_ID_DRV_OBSS_SCAN_PARAM:
        PRINTM(MMSG,"MLAN_EVENT_ID_DRV_OBSS_SCAN_PARAM \n");              
        break;

    case MLAN_EVENT_ID_FW_BW_CHANGED:
        PRINTM(MMSG,"MLAN_EVENT_ID_FW_BW_CHANGED \n");
        break;

    case MLAN_EVENT_ID_FW_DISCONNECTED:
        PRINTM(MMSG,"MLAN_EVENT_ID_FW_DISCONNECTED \n");
        priv->media_connected = MFALSE;
        ewl_os_memset(wrqu.ap_addr.sa_data, 0x00, ETH_ALEN);
		wrqu.ap_addr.sa_family = ARPHRD_ETHER;
        ewl_wireless_send_event(priv, SIOCGIWAP, &wrqu, NULL);
        priv->dis_connected_cb(NULL, 0);
        break;

    case MLAN_EVENT_ID_FW_MIC_ERR_UNI:
        PRINTM(MMSG,"MLAN_EVENT_ID_FW_MIC_ERR_UNI \n");
        break;

    case MLAN_EVENT_ID_FW_MIC_ERR_MUL:
        PRINTM(MMSG,"MLAN_EVENT_ID_FW_MIC_ERR_MUL \n");
        break;

    case MLAN_EVENT_ID_FW_BCN_RSSI_LOW:
        PRINTM(MMSG,"CUS_EVT_BEACON_RSSI_LOW \n");
        break;

    case MLAN_EVENT_ID_FW_BCN_RSSI_HIGH:
        PRINTM(MMSG,"CUS_EVT_BEACON_RSSI_HIGH \n");
        break;

    case MLAN_EVENT_ID_FW_BCN_SNR_LOW:
        PRINTM(MMSG,"CUS_EVT_BEACON_SNR_LOW \n");
        break;

    case MLAN_EVENT_ID_FW_BCN_SNR_HIGH:
        PRINTM(MMSG,"CUS_EVT_BEACON_SNR_HIGH\n");
        break;

    case MLAN_EVENT_ID_FW_MAX_FAIL:
        PRINTM(MMSG,"CUS_EVT_MAX_FAIL\n");
        break;

    case MLAN_EVENT_ID_FW_DATA_RSSI_LOW:
        PRINTM(MMSG,"CUS_EVT_DATA_RSSI_LOW\n");
        break;

    case MLAN_EVENT_ID_FW_DATA_SNR_LOW:
        PRINTM(MMSG,"CUS_EVT_DATA_SNR_LOW\n");
        break;

    case MLAN_EVENT_ID_FW_DATA_RSSI_HIGH:
        PRINTM(MMSG,"CUS_EVT_DATA_RSSI_HIGH\n");
        break;

    case MLAN_EVENT_ID_FW_DATA_SNR_HIGH:
        PRINTM(MMSG,"CUS_EVT_DATA_SNR_HIGH\n");
        break;

    case MLAN_EVENT_ID_FW_LINK_QUALITY:
        PRINTM(MMSG,"MLAN_EVENT_ID_FW_LINK_QUALITY \n");
        break;

    case MLAN_EVENT_ID_FW_PORT_RELEASE:
        PRINTM(MMSG,"MLAN_EVENT_ID_FW_PORT_RELEASE\n");
        priv->connected_cb(NULL, 0);
        break;

    case MLAN_EVENT_ID_FW_PRE_BCN_LOST:
        PRINTM(MMSG," CUS_EVT_PRE_BEACON_LOST \n");
        break;

    case MLAN_EVENT_ID_FW_WMM_CONFIG_CHANGE:
        PRINTM(MMSG,"WMM_CONFIG_CHANGE_INDICATION\n");
        break;

    case MLAN_EVENT_ID_DRV_REPORT_STRING:
        PRINTM(MMSG,"MLAN_EVENT_ID_DRV_REPORT_STRING\n");
        break;
    case MLAN_EVENT_ID_FW_WEP_ICV_ERR:
        PRINTM(MMSG,"CUS_EVT_WEP_ICV_ERR\n");
        break;

    case MLAN_EVENT_ID_DRV_DEFER_HANDLING:
        PRINTM(MMSG,"MLAN_EVENT_ID_DRV_DEFER_HANDLING\n");
        break;

    case MLAN_EVENT_ID_FW_BG_SCAN:
        PRINTM(MMSG,"MLAN_EVENT_ID_FW_BG_SCAN \n");
        break;
    
    case MLAN_EVENT_ID_FW_STOP_TX:
        PRINTM(MMSG,"MLAN_EVENT_ID_FW_STOP_TX \n");
        break;
    
    case MLAN_EVENT_ID_FW_START_TX:
        PRINTM(MMSG,"MLAN_EVENT_ID_FW_START_TX \n");
        break;

    case MLAN_EVENT_ID_FW_CHANNEL_SWITCH_ANN:
        PRINTM(MMSG,"MLAN_EVENT_ID_FW_START_TX \n");
        break;
//
#endif /* STA_SUPPORT */
    case MLAN_EVENT_ID_FW_HS_WAKEUP:
		flprintf("MLAN_EVENT_ID_FW_HS_WAKEUP\n");
        /* simulate HSCFG_CANCEL command */
        PRINTM(MMSG,"MLAN_EVENT_ID_FW_START_TX \n");
        woal_cancel_hs(priv, MOAL_NO_WAIT);
        break;

    case MLAN_EVENT_ID_DRV_HS_ACTIVATED:
		flprintf("MLAN_EVENT_ID_DRV_HS_ACTIVATED\n");
        PRINTM(MMSG,"MLAN_EVENT_ID_DRV_HS_DEACTIVATED \n");
        break;

    case MLAN_EVENT_ID_DRV_HS_DEACTIVATED:
		flprintf("MLAN_EVENT_ID_DRV_HS_DEACTIVATED\n");
        PRINTM(MMSG,"MLAN_EVENT_ID_DRV_HS_DEACTIVATED \n");
        break;

#ifdef UAP_SUPPORT
    case MLAN_EVENT_ID_UAP_FW_BSS_START:
        PRINTM(MMSG,"MLAN_EVENT_ID_UAP_FW_BSS_START\n");
        priv->bss_started = MTRUE;
        ewl_os_memcpy(priv->current_addr, pmevent->event_buf + 6, ETH_ALEN);
        break;
         
    case MLAN_EVENT_ID_UAP_FW_BSS_ACTIVE:
        PRINTM(MMSG,"MLAN_EVENT_ID_FW_BSS_ACTIVE\n");
        priv->media_connected = MTRUE;
        priv->connected_cb(NULL, 0);
        break;
        
    case MLAN_EVENT_ID_UAP_FW_BSS_IDLE:
        PRINTM(MMSG,"MLAN_EVENT_ID_UAP_FW_BSS_IDLE \n");
        priv->media_connected = MFALSE;
        priv->dis_connected_cb(NULL, 0);
        break;
        
	case MLAN_EVENT_ID_UAP_FW_STA_CONNECT:
		PRINTM(MMSG,"MLAN_EVENT_ID_UAP_FW_STA_CONNECT\n");
		priv->connected_cb(pmevent->event_buf, pmevent->event_len);
		break;

	case MLAN_EVENT_ID_UAP_FW_STA_DISCONNECT:
		PRINTM(MMSG,"MLAN_EVENT_ID_UAP_FW_STA_DISCONNECT\n");
		priv->dis_connected_cb(pmevent->event_buf, pmevent->event_len);
		break;

#endif /* UAP_SUPPORT */


    case MLAN_EVENT_ID_DRV_PASSTHRU:
//		flprintf("MLAN_EVENT_ID_DRV_PASSTHRU\n");
        PRINTM(MMSG,"MLAN_EVENT_ID_DRV_PASSTHRU \n");
        break;

    case MLAN_EVENT_ID_DRV_MEAS_REPORT:
		flprintf("MLAN_EVENT_ID_DRV_MEAS_REPORT\n");
        break;

    default:
		flprintf("pmevent->event_id(0x%08X)\n", pmevent->event_id);
        break;
    }
  done:
    LEAVE();
  return MLAN_STATUS_SUCCESS;
}

/** 
 *  @brief This function prints the debug message in mlan
 *
 *  @param pmoal_handle Pointer to the MOAL context
 *  @param level	debug level
 *  @param pformat	point to string format buf
 *
 *  @return    		N/A 
 */
t_void
moal_print(IN t_void * pmoal_handle, IN t_u32 level, IN t_s8 * pformat, IN ...)
{
    
    MLAN_UNUSED_ARG(pmoal_handle);
    MLAN_UNUSED_ARG(level);
    MLAN_UNUSED_ARG(pformat);
#ifdef	DEBUG_LEVEL1
    va_list args;
    
    if (level & MHEX_DUMP) 
    {
        t_u8 *buf = NULL;
        int len = 0;
        
        va_start(args, pformat);
        buf = (t_u8 *) va_arg(args, t_u8 *);
        len = (int) va_arg(args, int);
        va_end(args);
    
#ifdef DEBUG_LEVEL2
        if (level & MINFO)
            HEXDUMP((char *) pformat, buf, len);
        else
#endif /* DEBUG_LEVEL2 */
        {
            if (level & MCMD_D)
                DBG_HEXDUMP(MCMD_D, (char *) pformat, buf, len);
            if (level & MDAT_D)
                DBG_HEXDUMP(MDAT_D, (char *) pformat, buf, len);
            if (level & MIF_D)
                DBG_HEXDUMP(MIF_D, (char *) pformat, buf, len);
            if (level & MFW_D)
                DBG_HEXDUMP(MFW_D, (char *) pformat, buf, len);
        }
    } 
    else if(drvdbg & level) 
    {
        va_start(args, pformat);
        ewl_os_vprint(pformat, args);
        va_end(args);
    }
#endif /* DEBUG_LEVEL1 */

}

/** 
 *  @brief This function asserts the existence of the passed argument
 *
 *  @param pmoal_handle     A pointer to moal_private structure
 *  @param cond             Condition to check
 *
 *  @return    		        N/A
 */
t_void
moal_assert(IN t_void * pmoal_handle, IN t_u32 cond)
{
    MLAN_UNUSED_ARG(pmoal_handle);
    if (!cond)
    {
        while(1) ewl_os_printf("Assert failed: Panic! \n");
        
    }    
}

/**
 *  @brief This function indicate tcp ack tx
 *
 *  @param pmoal_handle     A pointer to moal_private structure
 *  @param pmbuf            Pointer to the mlan buffer structure
 *
 *  @return                 N/A
 */
t_void
moal_tcp_ack_tx_ind(IN t_void * pmoal_handle, IN pmlan_buffer pmbuf)
{
  /*
    nothing to do
   */
	UNUSED(pmoal_handle);UNUSED(pmbuf);
}
