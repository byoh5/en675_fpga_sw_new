#include "dev.h"
#include "enx_freertos.h"

#include <string.h> // for memset

#if (LOAD_FS_SDCARD==1)
#include "stream_buffering.h"
#include "sdcard.h"

/**
	@param
	@num : number of stream 

	@return 
		ENX_OK   : ok
		ENX_FAIL : FAIL
*/
ENX_OKFAIL
sbuf_init(sbuf_t *psbuf, int tot_num)
{
	memset(psbuf, 0, sizeof(sbuf_t));

	psbuf->head = 0;
	psbuf->tail = 0;
	psbuf->tot_num = tot_num;
	psbuf->data = (sbuf_data *)pvPortCalloc(sizeof(sbuf_data), tot_num);

	if (psbuf->data != NULL) {
		return ENX_OK;
	} else {
		return ENX_FAIL;
	}
}

ENX_OKFAIL
sbuf_put(sbuf_t *psbuf, u64 addr, u32 size, u32 flags, u32 type)
{
	if (cQueue_isfull(psbuf) == ENX_OK) { // 이 조건문에 진입하면, 다음 데이터가 I가 아니라면 의미 없으므로 밀어버린다.
//		if (getSDState() == sd_SAVE) {
//			printf("sbuf(0x%08X)_put full! type:%d\r\n", psbuf, type);
//		}
		num_loop(psbuf->head, psbuf->tot_num);
#if 1
		while (cQueue_isempty(psbuf) != ENX_OK) {
			if (psbuf->data[psbuf->head].flags != 1) {
				num_loop(psbuf->head, psbuf->tot_num);
			} else {
				break;
			}
		}
#endif
	}

	psbuf->data[psbuf->tail].addr = addr;
	psbuf->data[psbuf->tail].size = size;
	psbuf->data[psbuf->tail].flags = flags;
	psbuf->data[psbuf->tail].type = type;

	num_loop(psbuf->tail, psbuf->tot_num);

	return ENX_OK;
}

ENX_OKFAIL
sbuf_get(sbuf_t *psbuf, u64 *addr, u32 *size, u32 *flags, u32 *type)
{
	ENX_OKFAIL bRes = ENX_FAIL;
	if (cQueue_isempty(psbuf) == ENX_FAIL) {
		portENTER_CRITICAL();

		*addr = psbuf->data[psbuf->head].addr;
		*size = psbuf->data[psbuf->head].size;
		*flags = psbuf->data[psbuf->head].flags;
		*type = psbuf->data[psbuf->head].type;

		num_loop(psbuf->head, psbuf->tot_num);

		bRes = ENX_OK;

		portEXIT_CRITICAL();
	}
	return bRes;
}

ENX_OKFAIL
sbuf_peek(sbuf_t *psbuf, u64 *addr, u32 *size, u32 *flags, u32 *type)
{
	ENX_OKFAIL bRes = ENX_FAIL;
	if (cQueue_isempty(psbuf) == ENX_FAIL) {
		portENTER_CRITICAL();

		*addr = psbuf->data[psbuf->head].addr;
		*size = psbuf->data[psbuf->head].size;
		*flags = psbuf->data[psbuf->head].flags;
		*type = psbuf->data[psbuf->head].type;

		bRes = ENX_OK;

		portEXIT_CRITICAL();
	}
	return bRes;
}

ENX_OKFAIL
sbuf_freebuffer(sbuf_t *psbuf)
{
	portENTER_CRITICAL();

	psbuf->head = 0;
	psbuf->tail = 0;
	memset(psbuf->data, 0, sizeof(sbuf_data)*psbuf->tot_num);

	portEXIT_CRITICAL();

	return ENX_OK;
}

ENX_OKFAIL
sbuf_free(sbuf_t *psbuf)
{
	portENTER_CRITICAL();

	psbuf->head = 0;
	psbuf->tail = 0;
	psbuf->tot_num = 0;
	if (psbuf->data) {
		vPortFree(psbuf->data);
	}

	memset(psbuf, 0, sizeof(sbuf_t));

	portEXIT_CRITICAL();

	return ENX_OK;
}
#endif
