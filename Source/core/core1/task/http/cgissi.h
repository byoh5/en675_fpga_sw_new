#ifndef CGISSI_H
#define CGISSI_H

#define XML_NA		"N/A"
#define XML_DISABLE	"<DISABLE>1</DISABLE>"
#define CRLF		"\r\n"

enum{
	UIN_OK				= 0x00000001,	// 1
	UIN_UR_TIMEZONE		= 0x00000002,	//
	UIN_UR_DATETIME 	= 0x00000004,	//
	UIN_UR_SD_SAVE		= 0x00000008,	//
};

#include "cgissi_en675.h"
//#include "cgissi_isp.h"
//#include "cgissi_wifi.h"

#if LWIP_HTTPD_CGI
void cgi_init(void);
#endif

#if LWIP_HTTPD_SSI
void ssi_init(void);
#endif

#endif /* CGISSI_H */
