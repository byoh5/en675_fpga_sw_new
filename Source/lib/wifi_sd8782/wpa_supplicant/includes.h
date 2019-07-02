/*
 * wpa_supplicant/hostapd - Default include files
 * Copyright (c) 2005-2006, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 *
 * This header file is included into all C files so that commonly used header
 * files can be selected with OS specific #ifdefs in one place instead of
 * having to have OS/C library specific selection in many files.
 */

#ifndef INCLUDES_H
#define INCLUDES_H

/* Include possible build time configuration before including anything else */

#include <ewl_os.h>
//#include <types.h>

extern int errno;

#endif /* INCLUDES_H */
