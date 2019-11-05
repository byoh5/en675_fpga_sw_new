#ifndef __HTTPD_AUTHORIZATION_H__
#define __HTTPD_AUTHORIZATION_H__

#include "lwip/apps/httpd_opts.h"

#if LWIP_HTTPD_SUPPORT_AUTHORIZATION

#if (LWIP_HTTPD_SUPPORT_AUTHORIZATION==LWIP_HTTPD_AUTH_BASIC)
#define HTTP11_AUTORIZATION_TEXT "Authorization: Basic "
#define HTTP11_AUTORIZATION_LEN 21
#elif (LWIP_HTTPD_SUPPORT_AUTHORIZATION==LWIP_HTTPD_AUTH_DIGEST)
#define HTTP11_AUTORIZATION_TEXT "Authorization: Digest "
#define HTTP11_AUTORIZATION_LEN 22
#endif

#define CRLF "\r\n"

extern int http_auth_process(int isPost, char *data, int data_len);
#endif

#endif	/* __HTTPD_AUTHORIZATION_H__ */
