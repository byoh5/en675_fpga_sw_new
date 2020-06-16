#ifndef __NETWORKIF_H__
#define __NETWORKIF_H__

#include "enx_lwip.h"
#include "ethernetif.h"
#include "wifiif.h"

//*************************************************************************************************
// Macro
//-------------------------------------------------------------------------------------------------
//
typedef enum { // netif array index
#if defined(__ETHERNET__)
	enlETHERNET,
#endif
#if defined(__WIFI__)
	enlWIFISTA,
	enlWIFIUAP,
#endif
	enlEnd,
} NETIFLIST;

typedef struct {
	char ifname[12];
	struct netif *_netif;
	TaskHandle_t xrx_notity;
	TaskHandle_t link_notity;
	NETIFLIST iftype;
} netif_state_t;

#define wltx_init_lock()    vSemaphoreCreateBinary(xtxwl_sem)
#define wltx_lock()         xSemaphoreTake(xtxwl_sem, portMAX_DELAY)
#define wltx_unlock()       xSemaphoreGive(xtxwl_sem)

#define wlrx_init_lock()    vSemaphoreCreateBinary(xrxwl_sem)
#define wlrx_lock()         xSemaphoreTake(xrxwl_sem, portMAX_DELAY)
#define wlrx_unlock()       xSemaphoreGive(xrxwl_sem)

#define NETRX_ALIGN_SIZE   			64
#define NETRX_ALIGN(x) 				(((x) + NETRX_ALIGN_SIZE - 1) & ~(NETRX_ALIGN_SIZE - 1))

#define NETRX_BUF_GAP				1536
#define NETRX_BUF_COUNT				256		// 1packet(NETRX_ALIGN(1514)==1536) * 256 = 393,216B

#define NETTX_BUF_GAP				1536
#define NETTX_BUF_COUNT				128		// 1packet(NETRX_ALIGN(1514)==1536) * 128 = 196,608B

#define cNRQueue_isfull(cQueue)		(((((cQueue)->tail+1) % MSG_NETRX_NUM) == (cQueue)->head) ? ENX_OK : ENX_FAIL)
#define cNRQueue_isempty(cQueue)	(((cQueue)->head == (cQueue)->tail) ? ENX_OK : ENX_FAIL)

typedef struct {
	BYTE *data;								// packet data address
	WORD lenth;								// packet lenth
	BYTE type;								// (1)Eth, (2)Wi-Fi, (3)Wi-Fi evt
	BYTE flag;								// unused
} pkt_info; // 16byte

typedef struct {
	struct {
		BYTE buffer[NETRX_BUF_GAP];
	} *pkt_data;
	BYTE *base;

	UINT tail;
	UINT head;
	pkt_info info[NETRX_BUF_COUNT]; // 16 * NETRX_BUF_COUNT = 4096byte
} QueueNetRx_t;

typedef struct {
	struct {
		BYTE buffer[NETTX_BUF_GAP];
	} *pkt_data;
	BYTE *base;
	UINT index;
} QueueNetTx_t;

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern err_t low_level_init(struct netif *netif, char name0, char name1);
extern void network_check_netif(void);
extern void network_default_netif(void);
extern void network_default_netif_get_ip(UINT *pIP);
extern void network_interface_link(NETIFLIST iftype, ENX_SWITCH u32Switch);
extern void network_interface_list(void);
extern void startLwip(void *arg);
extern ENX_OKFAIL network_igmp_join(char *strIP);
extern ENX_OKFAIL network_igmp_leave(char *strIP);

//-------------------------------------------------------------------------------------------------
// Variable
extern SemaphoreHandle_t xtxwl_sem;
extern SemaphoreHandle_t xrxwl_sem;

extern netif_state_t netif_state[];

#if defined(__ETHERNET__)
extern QueueNetRx_t qEthernetRX;
extern QueueNetTx_t qEthernetTX;
#endif

#endif // __NETWORKIF_H__
