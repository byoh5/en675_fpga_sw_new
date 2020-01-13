#ifndef __AUDTXRX_H__
#define __AUDTXRX_H__

#define ENX_AUDMEM_ALIGNMENT		(1024)
#define ENX_AUDMEM_ALIGN_SIZE(size)	(((size) + ENX_AUDMEM_ALIGNMENT - 1) & ~(ENX_AUDMEM_ALIGNMENT - 1))
#define ENX_AUDMEM_ALIGN(addr)		(((intptr_t)(addr) + ENX_AUDMEM_ALIGNMENT - 1) & ~(intptr_t)(ENX_AUDMEM_ALIGNMENT - 1))

#define AUD_RX_SIZE		0x400

typedef struct {
	BYTE *audrx_buffer;
	BYTE *audrx_malloc;
	UINT audrx_buffer_length;
	UINT audrx_malloc_length;
	UINT audrx_old_pos;
	UINT audrx_now_pos;

	BYTE *audtx_buffer;
	BYTE *audtx_malloc;
	UINT audtx_buffer_length;
	UINT audtx_malloc_length;
	UINT audtx_now_pos;
	UINT audtx_old_pos;

#if 0
	BYTE audbuffer[AUD_RX_SIZE];
	UINT aud_strm_base;
	UINT length;
	UINT over_length;
	UINT over_strm_base;
	UINT flag;
	UINT strm_base;
	UINT cnt;
#endif
	ULONG audrx_irq_count;
	ULONG audtx_irq_count;
} AudioPool; // sizeof => 1024 + 4*7 = 1052

extern void audtxrx_task(void* ctx);

extern AudioPool audpool;

#endif // __AUDTXRX_H__
