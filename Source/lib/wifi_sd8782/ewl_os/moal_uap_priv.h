/** @file  moal_uap_priv.h
  *
  * @brief This file contains definition for extended private IOCTL call.
  *
  * Copyright (C) 2010-2011, Marvell International Ltd.
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

/************************************************************************
Change log:
    08/06/2010: initial version
************************************************************************/

#ifndef _MOAL_UAP_PRIV_H_
#define _MOAL_UAP_PRIV_H_

/** Maximum buffer length for WOAL_UAP_SET_GET_256_CHAR */
#define MAX_BUF_LEN                 256

mlan_status
woal_uap_get_bss_param(moal_private * priv, mlan_uap_bss_param * sys_cfg,
                       t_u8 wait_option);

mlan_status
woal_uap_set_11n_status(mlan_uap_bss_param * sys_cfg, t_u8 action);

#endif /* _MOAL_UAP_PRIV_H_ */
