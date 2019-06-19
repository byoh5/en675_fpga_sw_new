/*
*********************************************************************************************************
*                                              uC/TCP-IP
*                                      The Embedded TCP/IP Suite
*
*                          (c) Copyright 2003-2008; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/TCP-IP is provided in source form for FREE evaluation, for educational
*               use or peaceful research.  If you plan on using uC/TCP-IP in a commercial
*               product you need to contact Micrium to properly license its use in your
*               product.  We provide ALL the source code for your convenience and to help
*               you experience uC/TCP-IP.  The fact that the source code is provided does
*               NOT mean that you can use it without paying a licensing fee.
*
*               Knowledge of the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                         NETWORK ASCII LIBRARY
*
* Filename      : net_ascii.h
* Version       : V1.92
* Programmer(s) : ITJ
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   NET_ASCII_MODULE
#define  NET_ASCII_EXT
#else
#define  NET_ASCII_EXT  extern
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  NET_ASCII_CHAR_LEN_DOT                            1
#define  NET_ASCII_CHAR_LEN_HYPHEN                         1
#define  NET_ASCII_CHAR_LEN_COLON                          1
#define  NET_ASCII_CHAR_LEN_NUL                            1


#define  NET_ASCII_CHAR_MIN_OCTET                          1

#define  NET_ASCII_CHAR_MAX_OCTET_08                       3
#define  NET_ASCII_CHAR_MAX_OCTET_16                       5
#define  NET_ASCII_CHAR_MAX_OCTET_32                      10


/*
*********************************************************************************************************
*                                    NETWORK ASCII ADDRESS DEFINES
*********************************************************************************************************
*/

#define  NET_ASCII_CHAR_MAX_OCTET_ADDR_MAC                 2

#ifdef   NET_IF_ADDR_SIZE_MAC
#define  NET_ASCII_NBR_OCTET_ADDR_MAC                      NET_IF_ADDR_SIZE_MAC
#else
#define  NET_ASCII_NBR_OCTET_ADDR_MAC                      6
#endif


#define  NET_ASCII_LEN_MAX_ADDR_MAC                    ((NET_ASCII_NBR_OCTET_ADDR_MAC      * NET_ASCII_CHAR_MAX_OCTET_ADDR_MAC) + \
                                                       ((NET_ASCII_NBR_OCTET_ADDR_MAC - 1) * NET_ASCII_CHAR_LEN_HYPHEN        ) + \
                                                         NET_ASCII_CHAR_LEN_NUL                                               )


#define  NET_ASCII_CHAR_MAX_OCTET_ADDR_IP                  3
#define  NET_ASCII_VAL_MAX_OCTET_ADDR_IP                 255

#define  NET_ASCII_NBR_OCTET_ADDR_IP                   (sizeof(INT32U))


#define  NET_ASCII_LEN_MAX_ADDR_IP                     ((NET_ASCII_NBR_OCTET_ADDR_IP      * NET_ASCII_CHAR_MAX_OCTET_ADDR_IP) + \
                                                       ((NET_ASCII_NBR_OCTET_ADDR_IP - 1) * NET_ASCII_CHAR_LEN_DOT          ) + \
                                                         NET_ASCII_CHAR_LEN_NUL                                             )


/*$PAGE*/
/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  NetASCII_Str_to_MAC(char     *paddr_mac_ascii,
                          char     *paddr_mac,
                          int      *perr);

void  NetASCII_MAC_to_Str(char     *paddr_mac,
                          char     *paddr_mac_ascii,
                          BOOL      hex_lower_case,
                          BOOL      hex_colon_sep,
                          int      *perr);


INT32U  NetASCII_Str_to_IP(char     *paddr_ip_ascii,
                           int      *perr);

void  NetASCII_IP_to_Str(INT32U     addr_ip,
                         char       *paddr_ip_ascii,
                         BOOL       lead_zeros,
                         int        *perr);

/*$PAGE*/
/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

