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
* Filename      : net_ascii.c
* Version       : V1.92
* Programmer(s) : ITJ
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    NET_ASCII_MODULE

#include "dev.h"
#include "net_ascii.h"

#include <string.h> // memset


/*$PAGE*/
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*$PAGE*/
/*
*********************************************************************************************************
*                                        NetASCII_Str_to_MAC()
*
* Description : Convert an Ethernet MAC address ASCII string to an Ethernet MAC address.
*
* Argument(s) : paddr_mac_ascii     Pointer to an ASCII string that contains a MAC address (see Note #1).
*
*               paddr_mac           Pointer to a memory buffer that will receive the converted MAC address
*                                       (see Note #2) :
*
*                                       MAC address represented by ASCII string,         if NO errors.
*
*                                       MAC address cleared to all zeros (see Note #2c), otherwise.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               NET_ASCII_ERR_NONE                  MAC address successfully converted.
*                               NET_ASCII_ERR_NULL_PTR              Argument 'paddr_mac_ascii'/'paddr_mac'
*                                                                       passed a NULL pointer.
*                               NET_ASCII_ERR_INVALID_LEN           Invalid ASCII string length.
*                               NET_ASCII_ERR_INVALID_CHAR          Invalid ASCII character.
*                               NET_ASCII_ERR_INVALID_CHAR_LEN      Invalid ASCII character length.
*                               NET_ASCII_ERR_INVALID_CHAR_SEQ      Invalid ASCII character sequence.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
*               This function is a network protocol suite application interface (API) function & MAY be 
*               called by application function(s).
*
* Note(s)     : (1) (a) (1) RFC #1700, Section 'ETHERNET VENDOR ADDRESS COMPONENTS' states that "Ethernet 
*                           addresses ... should be written hyphenated by octets (e.g., 12-34-56-78-9A-BC)".
*
*                       (2) In other words, the (Ethernet) MAC address notation separates six hexadecimal 
*                           octet values by the hyphen character ('-') or by the colon character (':').
*                           Each hexadecimal value represents one octet of the MAC address starting with 
*                           the most significant octet in network-order.
*
*                               MAC Address Examples :
*
*                                      MAC ADDRESS NOTATION        HEXADECIMAL EQUIVALENT
*
*                                       "00-1A-07-AC-22-09"     =     0x001A07AC2209
*                                       "76:4E:01:D2:8C:0B"     =     0x764E01D28C0B
*                                       "80-Db-fE-0b-34-52"     =     0X80DBFE0B3452
*                                        --             --              --        --
*                                        ^               ^              ^          ^
*                                        |               |              |          |
*                                       MSO             LSO            MSO        LSO
*
*                               where
*                                       MSO             Most  Significant Octet in MAC Address
*                                       LSO             Least Significant Octet in MAC Address
*
*
*                   (b) Therefore, the MAC address ASCII string MUST :
*
*                       (1) Include ONLY hexadecimal values & the hyphen character ('-') or the colon 
*                           character (':') ; ALL other characters are trapped as invalid, including 
*                           any leading or trailing characters.
*
*                       (2) (A) Include EXACTLY six hexadecimal values ...
*                           (B) (1) ... separated by                   ...
*                               (2) ... EXACTLY five                   ...
*                                   (a) hyphen characters or           ...
*                                   (b) colon  characters;             ...
*                           (C) ... & MUST be terminated with the NULL character.
*
*                       (3) Ensure that each hexadecimal value's number of digits does NOT exceed the
*                           maximum number of digits (2).
*
*                           (A) However, any hexadecimal value's number of digits MAY be less than the
*                               maximum number of digits.
*
*               (2) (a) The size of the memory buffer that will receive the converted MAC address MUST
*                       be greater than or equal to NET_ASCII_NBR_OCTET_ADDR_MAC.
*
*                   (b) MAC address memory buffer array accessed by octets.
*
*                   (c) MAC address memory buffer array cleared in case of any error(s).
*********************************************************************************************************
*/
/*$PAGE*/
void  NetASCII_Str_to_MAC (char    *paddr_mac_ascii,
                           char  *paddr_mac,
                           int     *perr)
{
    char  *pchar_cur;
    char  *pchar_prev;
    char  *paddr_cur;
    char   addr_octet_val;
    char   addr_octet_val_dig;
    char   addr_nbr_octet;
    char   addr_nbr_octet_dig;


                                                                        /* -------------- VALIDATE PTRS --------------- */
    if (paddr_mac == (char *)0) {
       *perr = ENX_FAIL;
        return;
    }

    if (paddr_mac_ascii == (char *)0) {
        memset((void     *)paddr_mac, 0,                                 /* Clr rtn addr on err (see Note #2c).          */
                (size_t)NET_ASCII_NBR_OCTET_ADDR_MAC);
       *perr = ENX_FAIL;
        return;
    }


/*$PAGE*/
                                                                        /* ----------- CONVERT MAC ADDR STR ----------- */
    pchar_cur          = (char   *)paddr_mac_ascii;
    pchar_prev         = (char   *)0;
    paddr_cur          = (char *)paddr_mac;
    addr_octet_val     =  0x00;
    addr_nbr_octet     =  0;
    addr_nbr_octet_dig =  0;

    while ((pchar_cur != (char *)0) &&                              /* Parse ALL non-NULL chars in ASCII str.       */
          (*pchar_cur != (char  )0)) {

        switch (*pchar_cur) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
                 addr_nbr_octet_dig++;                                  /* If nbr digs > max (see Note #1b3); ...       */
                 if (addr_nbr_octet_dig > NET_ASCII_CHAR_MAX_OCTET_ADDR_MAC) {
                     memset((void     *)paddr_mac, 0,                    /* ... clr rtn addr  (see Note #2c)   ...       */
                             (size_t)NET_ASCII_NBR_OCTET_ADDR_MAC);
                    *perr = ENX_FAIL;             /* ... & rtn err.                               */
                     return;
                 }
                                                                        
                 switch (*pchar_cur) {                                  /* Convert ASCII char into hex val.             */
                     case '0':
                     case '1':
                     case '2':
                     case '3':
                     case '4':
                     case '5':
                     case '6':
                     case '7':
                     case '8':
                     case '9':
                          addr_octet_val_dig = (char)(*pchar_cur - '0');
                          break;


                     case 'A':
                     case 'B':
                     case 'C':
                     case 'D':
                     case 'E':
                     case 'F':
                          addr_octet_val_dig = (char)(*pchar_cur - 'A' + 10);
                          break;


                     case 'a':
                     case 'b':
                     case 'c':
                     case 'd':
                     case 'e':
                     case 'f':
                          addr_octet_val_dig = (char)(*pchar_cur - 'a' + 10);
                          break;


                     default:                                           /* See Note #1b1.                               */
                          memset((void     *)paddr_mac, 0,               /* Clr rtn addr on err (see Note #2c).          */
                                  (size_t)NET_ASCII_NBR_OCTET_ADDR_MAC);
                         *perr = ENX_FAIL;
                          return;                                       /* Prevent 'break NOT reachable' warning.       */
                 }
                                                                        /* Merge ASCII char into hex val.               */
                 addr_octet_val <<= DEF_NIBBLE_NBR_BITS;
                 addr_octet_val  += addr_octet_val_dig;
                 break;

/*$PAGE*/
            case '-':
            case ':':
                 if (pchar_prev == (char *)0) {                     /* If NO prev char  (see Note #1b2B1); ...      */
                     memset((void     *)paddr_mac, 0,                     /* ... clr rtn addr (see Note #2c)     ...      */
                             (size_t)NET_ASCII_NBR_OCTET_ADDR_MAC);
                    *perr = ENX_FAIL;             /* ... & rtn err.                               */
                     return;
                 }

                 if ((*pchar_prev == (char)'-') ||                  /* If prev char a hyphen                   ...  */
                     (*pchar_prev == (char)':')) {                  /* ... or       a colon (see Note #1b2B1); ...  */
                     memset((void     *)paddr_mac, 0,                     /* ... clr rtn addr     (see Note #2c)     ...  */
                             (size_t)NET_ASCII_NBR_OCTET_ADDR_MAC);
                    *perr = ENX_FAIL;             /* ... & rtn err.                               */
                     return;
                 }

                 addr_nbr_octet++;
                 if (addr_nbr_octet >= NET_ASCII_NBR_OCTET_ADDR_MAC) {  /* If nbr octets > max (see Note #1b2A); ...    */
                     memset((void     *)paddr_mac, 0,                     /* ... clr rtn addr    (see Note #2c)    ...    */
                             (size_t)NET_ASCII_NBR_OCTET_ADDR_MAC);
                    *perr = ENX_FAIL;                  /* ... & rtn err.                               */
                     return;
                 }

                                                                        /* Merge hex octet val into MAC addr.           */
                *paddr_cur++        = addr_octet_val;
                 addr_octet_val     = 0x00;
                 addr_nbr_octet_dig = 0;
                 break;


            default:                                                    /* See Note #1b1.                               */
                 memset((void     *)paddr_mac, 0,                         /* Clr rtn addr on err (see Note #2c).          */
                         (size_t)NET_ASCII_NBR_OCTET_ADDR_MAC);
                *perr = ENX_FAIL;
                 return;                                                /* Prevent 'break NOT reachable' warning.       */
        }

        pchar_prev = pchar_cur;
        pchar_cur++;    
    }

    if (pchar_cur == (char *)0) {                                      /* If NULL ptr in ASCII str (see Note #1b1); .. */
        memset((void     *)paddr_mac, 0,                                  /* .. clr rtn addr          (see Note #2c)   .. */
                (size_t)NET_ASCII_NBR_OCTET_ADDR_MAC);
       *perr = ENX_FAIL;                                               /* .. & rtn err.                                */
        return;
    }

    if ((*pchar_prev == (char)'-') ||                               /* If last char a hyphen                   ..   */
        (*pchar_prev == (char)':')) {                               /* .. or        a colon (see Note #1b2B1); ..   */
        memset((void     *)paddr_mac, 0,                                  /* .. clr rtn addr      (see Note #2c)     ..   */
                (size_t)NET_ASCII_NBR_OCTET_ADDR_MAC);
       *perr = ENX_FAIL;                                            /* .. & rtn err.                                */
        return;
    }

    addr_nbr_octet++;
    if (addr_nbr_octet != NET_ASCII_NBR_OCTET_ADDR_MAC) {               /* If != nbr MAC addr octets (see Note #1b2A);  */
        memset((void     *)paddr_mac, 0,                                   /* .. clr rtn addr           (see Note #2c)  .. */
                (size_t)NET_ASCII_NBR_OCTET_ADDR_MAC);
       *perr = ENX_FAIL;                                                /* .. & rtn err.                                */
        return;
    }

                                                                        /* Merge hex octet val into final MAC addr.     */
   *paddr_cur = addr_octet_val;

   *perr      = ENX_OK;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                        NetASCII_MAC_to_Str()
*
* Description : Convert an Ethernet MAC address into an Ethernet MAC address ASCII string.
*
* Argument(s) : paddr_mac           Pointer to a memory buffer that contains the MAC address (see Note #2).
*
*               paddr_mac_ascii     Pointer to an ASCII character array that will receive the return MAC
*                                       address ASCII string from this function (see Note #1b).
*
*               hex_lower_case      Format alphabetic hexadecimal characters in lower case :
*
*                                       DEF_NO      Format alphabetic hexadecimal characters in upper case.
*                                       DEF_YES     Format alphabetic hexadecimal characters in lower case.
*
*               hex_colon_sep       Separate hexadecimal values with a colon character :
*
*                                       DEF_NO      Separate hexadecimal values with a hyphen character
*                                                       (see Note #1b1B2a).
*                                       DEF_YES     Separate hexadecimal values with a colon  character
*                                                       (see Note #1b1B2b).
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               NET_ASCII_ERR_NONE                  ASCII string successfully formatted.
*                               NET_ASCII_ERR_NULL_PTR              Argument 'paddr_mac'/'paddr_mac_ascii'
*                                                                       passed a NULL pointer.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
*               This function is a network protocol suite application interface (API) function & MAY be 
*               called by application function(s).
*
* Note(s)     : (1) (a) (1) RFC #1700, Section 'ETHERNET VENDOR ADDRESS COMPONENTS' states that "Ethernet 
*                           addresses ... should be written hyphenated by octets (e.g., 12-34-56-78-9A-BC)".
*
*                       (2) In other words, the (Ethernet) MAC address notation separates six hexadecimal 
*                           octet values by the hyphen character ('-') or by the colon character (':').
*                           Each hexadecimal value represents one octet of the MAC address starting with 
*                           the most significant octet in network-order.
*
*                               MAC Address Examples :
*
*                                      MAC ADDRESS NOTATION        HEXADECIMAL EQUIVALENT
*
*                                       "00-1A-07-AC-22-09"     =     0x001A07AC2209
*                                       "76:4E:01:D2:8C:0B"     =     0x764E01D28C0B
*                                       "80-Db-fE-0b-34-52"     =     0X80DBFE0B3452
*                                        --             --              --        --
*                                        ^               ^              ^          ^
*                                        |               |              |          |
*                                       MSO             LSO            MSO        LSO
*
*                               where
*                                       MSO             Most  Significant Octet in MAC Address
*                                       LSO             Least Significant Octet in MAC Address
*
*
*                   (b) (1) The return MAC address ASCII string :
*
*                           (A) Formats EXACTLY six hexadecimal values ...
*                           (B) (1) ... separated by                   ...
*                               (2) ... EXACTLY five                   ...
*                                   (a) hyphen characters or           ...
*                                   (b) colon  characters;             ...
*                           (C) ... & terminated with the NULL character.
*
*                       (2) The size of the ASCII character array that will receive the returned MAC address
*                           ASCII string MUST be greater than or equal to NET_ASCII_LEN_MAX_ADDR_MAC.
*
*               (2) (a) The size of the memory buffer that contains the MAC address SHOULD be greater than
*                       or equal to NET_ASCII_NBR_OCTET_ADDR_MAC.
*
*                   (b) MAC address memory buffer array accessed by octets.
*********************************************************************************************************
*/
/*$PAGE*/
void  NetASCII_MAC_to_Str (char   *paddr_mac,
                           char     *paddr_mac_ascii,
                           BOOL   hex_lower_case,
                           BOOL   hex_colon_sep,
                           int      *perr)
{
    char   *pchar;
    char   *paddr;
    char   addr_octet_val;
    char   addr_octet_dig_val;
    char   addr_octet_nbr_shifts;
    char   i;
    char   j;


                                                                        /* -------------- VALIDATE PTRS --------------- */
    if (paddr_mac == (char *)0) {
       *perr = ENX_FAIL;
        return;
    }
    if (paddr_mac_ascii == (char *)0) {
       *perr = ENX_FAIL;
        return;
    }


                                                                        /* ------------- CONVERT MAC ADDR ------------- */
    paddr = paddr_mac;
    pchar = paddr_mac_ascii;

    for (i = NET_ASCII_NBR_OCTET_ADDR_MAC; i > 0; i--) {                /* Parse ALL addr octets (see Note #1b1A).      */

        addr_octet_val = *paddr;

        for (j = NET_ASCII_CHAR_MAX_OCTET_ADDR_MAC; j > 0; j--) {       /* Parse ALL octet's hex digs.                  */
                                                                        /* Calc  cur octet's hex dig val.               */
            addr_octet_nbr_shifts = (j - 1) * DEF_NIBBLE_NBR_BITS;
            addr_octet_dig_val    = (addr_octet_val >> addr_octet_nbr_shifts) & DEF_NIBBLE_MASK;
                                                                        /* Insert    octet's hex val ASCII dig.         */
            if (addr_octet_dig_val < 10) {
               *pchar++ = (char)(addr_octet_dig_val + '0');
                
            } else {
                if (hex_lower_case != ENX_YES) {
                   *pchar++ = (char)(addr_octet_dig_val - 10 + 'A');
                } else {
                   *pchar++ = (char)(addr_octet_dig_val - 10 + 'a');
                }
            }
        }

        if (i > 1) {                                                    /* If NOT on last octet,                    ..  */
            if (hex_colon_sep != ENX_YES) {
               *pchar++ = '-';                                          /* .. insert hyphen char (see Note #1b1B2a) ..  */
            } else {
               *pchar++ = ':';                                          /* ..     or colon  char (see Note #1b1B2b).    */
            }
        }

        paddr++;
    }

   *pchar = (char)0;                                                /* Append NULL char (see Note #1b1C).           */

   *perr  =  ENX_OK;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                        NetASCII_Str_to_IP()
*
* Description : Convert an IPv4 address ASCII string in dotted-decimal notation to a network protocol 
*                   IPv4 address in host-order.
*
* Argument(s) : paddr_ip_ascii      Pointer to an ASCII string that contains a dotted-decimal IPv4 address
*                                       (see Note #1).
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               NET_ASCII_ERR_NONE                  IPv4 address successfully converted.
*                               NET_ASCII_ERR_NULL_PTR              Argument 'paddr_ip_ascii' passed a
*                                                                       NULL pointer.
*                               NET_ASCII_ERR_INVALID_LEN           Invalid ASCII string length.
*                               NET_ASCII_ERR_INVALID_CHAR          Invalid ASCII character.
*                               NET_ASCII_ERR_INVALID_CHAR_LEN      Invalid ASCII character length.
*                               NET_ASCII_ERR_INVALID_CHAR_VAL      Invalid ASCII character value.
*                               NET_ASCII_ERR_INVALID_CHAR_SEQ      Invalid ASCII character sequence.
*
* Return(s)   : Host-order IPv4 address represented by ASCII string, if NO errors.
*
*               0,                                    otherwise.
*
* Caller(s)   : Application.
*
*               This function is a network protocol suite application interface (API) function & MAY be 
*               called by application function(s).
*
* Note(s)     : (1) (a) (1) RFC #1983 states that "dotted decimal notation ... refers [to] IP addresses of the
*                           form A.B.C.D; where each letter represents, in decimal, one byte of a four byte IP
*                           address".
*
*                       (2) In other words, the dotted-decimal IP address notation separates four decimal octet 
*                           values by the dot, or period, character ('.').  Each decimal value represents one 
*                           octet of the IP address starting with the most significant octet in network-order.
*
*                               IP Address Examples :
*
*                                      DOTTED DECIMAL NOTATION     HEXADECIMAL EQUIVALENT
*
*                                           127.0.0.1           =       0x7F000001
*                                           192.168.1.64        =       0xC0A80140
*                                           255.255.255.0       =       0xFFFFFF00
*                                           ---         -                 --    --
*                                            ^          ^                 ^      ^
*                                            |          |                 |      |
*                                           MSO        LSO               MSO    LSO
*
*                                   where
*                                           MSO        Most  Significant Octet in Dotted Decimal IP Address
*                                           LSO        Least Significant Octet in Dotted Decimal IP Address
*
*
*                   (b) Therefore, the dotted-decimal IP address ASCII string MUST :
*
*                       (1) Include ONLY decimal values & the dot, or period, character ('.') ; ALL other
*                           characters are trapped as invalid, including any leading or trailing characters.
*
*                       (2) (A) Include EXACTLY four decimal values   ...
*                           (B) (1) ... separated by                  ...
*                               (2) ... EXACTLY three dot characters; ...
*                           (C) ... & MUST be terminated with the NULL character.
*
*                       (3) Ensure that each decimal value's number of decimal digits, including leading 
*                           zeros, does NOT exceed      the maximum number of digits (3).
*
*                           (A) However, any decimal value's number of decimal digits, including leading 
*                               zeros, MAY be less than the maximum number of digits.
*
*                       (4) Ensure that each decimal value does NOT exceed the maximum octet value (255).
*
*               (2) To avoid possible integer arithmetic overflow, the IP address octet arithmetic result 
*                   MUST be declared as an integer data type with a greater resolution -- i.e. greater 
*                   number of bits -- than the IP address octet data type(s).
*********************************************************************************************************
*/
/*$PAGE*/
INT32U  NetASCII_Str_to_IP (char  *paddr_ip_ascii,
                                 int   *perr)
{
    char     *pchar_cur;
    char     *pchar_prev;
    INT32U   addr_ip;
    INT16U   addr_octet_val;                                       /* See Note #2.                                 */
    INT8U    addr_nbr_octet;
    char     addr_nbr_octet_dig;


                                                                        /* --------------- VALIDATE PTR --------------- */
    if (paddr_ip_ascii == (char *)0) {
       *perr =  ENX_FAIL;
        return (0);
    }


                                                                        /* ----------- CONVERT IP ADDR STR ------------ */
    pchar_cur          = (char *)paddr_ip_ascii;
    pchar_prev         = (char *)0;
    addr_ip            =  0;
    addr_octet_val     =  0x0000;
    addr_nbr_octet     =  0;
    addr_nbr_octet_dig =  0;

    while ((pchar_cur != (char *)0) &&                              /* Parse ALL non-NULL chars in ASCII str.       */
          (*pchar_cur != (char  )0)) {

        switch (*pchar_cur) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                 addr_nbr_octet_dig++;                                  /* If nbr digs > max (see Note #1b3), ...       */
                 if (addr_nbr_octet_dig > NET_ASCII_CHAR_MAX_OCTET_ADDR_IP) {
                    *perr =  ENX_FAIL;            /* ... rtn err.                                 */
                     return (0);
                 }

                                                                        /* Convert & merge ASCII char into decimal val. */
                 addr_octet_val *= (INT16U)10;
                 addr_octet_val += (INT16U)(*pchar_cur - '0');

                 if (addr_octet_val > NET_ASCII_VAL_MAX_OCTET_ADDR_IP) {/* If octet val > max (see Note #1b4), ...      */
                    *perr =  ENX_FAIL;            /* ... rtn err.                                 */
                     return (0);
                 }
                 break;

 
            case '.':
                 if (pchar_prev == (char *)0) {                     /* If NO prev char     (see Note #1b2B1), ...   */
                    *perr =  ENX_FAIL;            /* ... rtn err.                                 */
                     return (0);
                 }

                 if (*pchar_prev == (char)'.') {                    /* If prev char a dot  (see Note #1b2B1), ...   */
                     *perr =  ENX_FAIL;           /* ... rtn err.                                 */
                      return (0);
                 }

                 addr_nbr_octet++;
                 if (addr_nbr_octet >= NET_ASCII_NBR_OCTET_ADDR_IP) {   /* If nbr octets > max (see Note #1b2A), ...    */
                    *perr =  ENX_FAIL;                 /* ... rtn err.                                 */
                     return (0);
                 }

                                                                        /* Merge decimal octet val into IP addr.        */
                 addr_ip            <<=  DEF_OCTET_NBR_BITS;
                 addr_ip             += (INT32U)addr_octet_val;
                 addr_octet_val       =  0x0000;
                 addr_nbr_octet_dig   =  0;
                 break;


            default:                                                    /* See Note #1b1.                               */
                *perr =  ENX_FAIL;
                 return (0);                             /* Prevent 'break NOT reachable' warning.       */
        }

        pchar_prev = pchar_cur;
        pchar_cur++;    
    }

/*$PAGE*/
    if (pchar_cur == (char *)0) {                                   /* If NULL ptr in ASCII str (see Note #1b1), .. */
       *perr =  ENX_FAIL;                                           /* .. rtn err.                                  */
        return (0);
    }

    if (*pchar_prev == (char)'.') {                                 /* If last char a dot (see Note #1b2B1), ..     */
        *perr =  ENX_FAIL;                                          /* .. rtn err.                                  */
         return (0);
    }

    addr_nbr_octet++;
    if (addr_nbr_octet != NET_ASCII_NBR_OCTET_ADDR_IP) {                /* If != nbr IP addr octets (see Note #1b2A),   */
       *perr =  ENX_FAIL;                                               /* .. rtn err.                                  */
        return (0);
    }

                                                                        /* Merge decimal octet val into final IP addr.  */
    addr_ip <<=  DEF_OCTET_NBR_BITS;
    addr_ip  += (INT32U)addr_octet_val;

   *perr      =  ENX_OK;

    return (addr_ip);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                        NetASCII_IP_to_Str()
*
* Description : Convert a network protocol IPv4 address in host-order into an IPv4 address ASCII string 
*                   in dotted-decimal notation.
*
* Argument(s) : addr_ip             IPv4 address.
*
*               paddr_ip_ascii      Pointer to an ASCII character array that will receive the return IP
*                                       address ASCII string from this function (see Note #1b).
*
*               lead_zeros          Prepend leading zeros option (see Note #2) :
*
*                                       DEF_NO      Do NOT prepend leading zeros to each decimal octet value.
*                                       DEF_YES            Prepend leading zeros to each decimal octet value.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               NET_ASCII_ERR_NONE                  ASCII string successfully formatted.
*                               NET_ASCII_ERR_NULL_PTR              Argument 'paddr_ip_ascii' passed a
*                                                                       NULL pointer.
*                               NET_ASCII_ERR_INVALID_CHAR_LEN      Invalid ASCII character length.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
*               This function is a network protocol suite application interface (API) function & MAY be 
*               called by application function(s).
*
* Note(s)     : (1) (a) (1) RFC #1983 states that "dotted decimal notation ... refers [to] IP addresses of the
*                           form A.B.C.D; where each letter represents, in decimal, one byte of a four byte IP
*                           address".
*
*                       (2) In other words, the dotted-decimal IP address notation separates four decimal octet 
*                           values by the dot, or period, character ('.').  Each decimal value represents one 
*                           octet of the IP address starting with the most significant octet in network-order.
*
*                               IP Address Examples :
*
*                                      DOTTED DECIMAL NOTATION     HEXADECIMAL EQUIVALENT
*
*                                           127.0.0.1           =       0x7F000001
*                                           192.168.1.64        =       0xC0A80140
*                                           255.255.255.0       =       0xFFFFFF00
*                                           ---         -                 --    --
*                                            ^          ^                 ^      ^
*                                            |          |                 |      |
*                                           MSO        LSO               MSO    LSO
*
*                                   where
*                                           MSO        Most  Significant Octet in Dotted Decimal IP Address
*                                           LSO        Least Significant Octet in Dotted Decimal IP Address
*
*
*                   (b) (1) The return dotted-decimal IP address ASCII string :
*
*                           (A) Formats EXACTLY four decimal values   ...
*                           (B) (1) ... separated by                  ...
*                               (2) ... EXACTLY three dot characters; ...
*                           (C) ... & terminated with the NULL character.
*
*                       (2) The size of the ASCII character array that will receive the returned IP address 
*                           ASCII string SHOULD be greater than or equal to NET_ASCII_LEN_MAX_ADDR_IP.
*
*               (2) (a) Leading zeros option prepends leading '0's prior to the first non-zero digit in each
*                       decimal octet value.  The number of leading zeros is such that the decimal octet's 
*                       number of decimal digits is equal to the maximum number of digits (3).
*
*                   (b) (1) If leading zeros option DISABLED              ...
*                       (2) ... & the decimal value of the octet is zero; ...
*                       (3) ... then one digit of '0' value is formatted.
*
*                       This is NOT a leading zero; but a single decimal digit of '0' value.
*********************************************************************************************************
*/
/*$PAGE*/
void  NetASCII_IP_to_Str (INT32U   addr_ip,
                          char     *paddr_ip_ascii,
                          BOOL      lead_zeros,
                          int      *perr)
{
  char    *pchar;
  INT8U   base_10_val_start;
  INT8U   base_10_val;
  INT8U   addr_octet_nbr_shifts;
  INT8U   addr_octet_val;
  INT8U   addr_octet_dig_nbr;
  INT8U   addr_octet_dig_val;
  INT8U   i;


                                                                      /* -------------- VALIDATE PTR ---------------- */
  if (paddr_ip_ascii == (char *)0) {
     *perr = ENX_FAIL;
     printf("err \r\n");
      return;
  }
                                                                      /* ------------ VALIDATE NBR CHAR ------------- */
#if ((NET_ASCII_CHAR_MAX_OCTET_ADDR_IP < NET_ASCII_CHAR_MIN_OCTET   ) || \
   (NET_ASCII_CHAR_MAX_OCTET_ADDR_IP > NET_ASCII_CHAR_MAX_OCTET_08))
 *paddr_ip_ascii = (char)0;
 *perr           =  ENX_FAIL;
  printf("err \r\n");
  return;
#endif


                                                                      /* ------------- CONVERT IP ADDR -------------- */
  pchar = paddr_ip_ascii;


  base_10_val_start = 1;    
  for (i = 1; i < NET_ASCII_CHAR_MAX_OCTET_ADDR_IP; i++) {            /* Calc starting dig val.                       */
      base_10_val_start *= 10;
  }


  for (i = NET_ASCII_NBR_OCTET_ADDR_IP; i > 0; i--) {                 /* Parse ALL addr octets (see Note #1b1A).      */
                                                                      /* Calc  cur addr octet val.                    */
    addr_octet_nbr_shifts = (i - 1) * DEF_OCTET_NBR_BITS;
    addr_octet_val        = (INT8U)((addr_ip >> addr_octet_nbr_shifts) & DEF_OCTET_MASK);

    base_10_val           =  base_10_val_start;
    while (base_10_val > 0) {                                         /* Parse ALL octet digs.                        */
        addr_octet_dig_nbr = addr_octet_val / base_10_val;

        if ((addr_octet_dig_nbr >  0) ||                              /* If octet dig val > 0,                     .. */
            (base_10_val        == 1) ||                              /* .. OR on least-significant octet dig,     .. */
            (lead_zeros == ENX_YES)) {                                /* .. OR lead zeros opt ENABLED (see Note #2),  */
                                                                      /* .. calc & insert octet val ASCII dig.        */
             addr_octet_dig_val = (INT8U)(addr_octet_dig_nbr % 10 );
            *pchar++            = (char  )(addr_octet_dig_val + '0');
        }

        base_10_val /= 10;                                          /* Shift to next least-significant octet dig.   */
    }

    if (i > 1) {                                                    /* If NOT on last octet, ...                    */
       *pchar++ = '.';                                              /* ... insert a dot char (see Note #1b1B2).     */
    }
  }

 *pchar = (char)0;                                                /* Append NULL char (see Note #1b1C).           */

 *perr  =  ENX_OK;
}

