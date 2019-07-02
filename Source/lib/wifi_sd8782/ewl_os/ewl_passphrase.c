
#include <ewl_os.h>
#include <errno.h> //for ENOMEM

#include "wireless.h"
#include "moal_main.h"
#include "moal_sdio.h"
#include "moal_util.h"
#include "moal_priv.h"
#include "moal_wext.h"
#include "ewl_passphrase.h"

/*
    set passphrase with ssid to embeded supplicant 
    @ctx        : point to instance of moal_private
    @passphrase : input by user.
    
    @ return : '0' means success else fail. 
*/

int
ewl_set_passphrase(void *ctx, ewl_passphrase_t *passphrase)
{
    int ret = 0;
    moal_private * priv = ctx;
    mlan_ds_sec_cfg *sec = NULL;
    mlan_ioctl_req *req = NULL;
    
    req = woal_alloc_mlan_ioctl_req(sizeof(mlan_ds_sec_cfg));
    if (req == NULL) {
        ret = -ENOMEM;
        goto done;
    }
    
    sec = (mlan_ds_sec_cfg *) req->pbuf;
    sec->sub_command = MLAN_OID_SEC_CFG_PASSPHRASE;
    req->req_id = MLAN_IOCTL_SEC_CFG;
    req->action = MLAN_ACT_SET;
    
    sec->param.passphrase.ssid.ssid_len = passphrase->ssid_len;
    ewl_os_strncpy((char *) sec->param.passphrase.ssid.ssid, 
            passphrase->ssid,
            passphrase->ssid_len);
    
    PRINTM(MMSG,"ssid=%s, len=%d\n", sec->param.passphrase.ssid.ssid,
    (int) sec->param.passphrase.ssid.ssid_len);
    
    sec->param.passphrase.psk_type = MLAN_PSK_PASSPHRASE;
    ewl_os_strcpy(sec->param.passphrase.psk.passphrase.passphrase,
           passphrase->passphrase);
    sec->param.passphrase.psk.passphrase.passphrase_len = passphrase->passphrase_len;
    PRINTM(MMSG,"passphrase=%s, len=%d\n",
            sec->param.passphrase.psk.passphrase.passphrase,
            (int) sec->param.passphrase.psk.passphrase.passphrase_len);
            
    if (MLAN_STATUS_SUCCESS != woal_request_ioctl(priv, req, MOAL_IOCTL_WAIT))
    {
        ret = -EFAULT;
        goto done;
    }
    
done:
    if (req)
        ewl_os_free(req);                
}

