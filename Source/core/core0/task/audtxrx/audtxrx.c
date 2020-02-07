#include "dev.h"
#include "enx_freertos.h"

#ifdef __AUDIO__
#include "enx_stream.h"
#include "enx_record.h"
#include "audtxrx.h"

//#include "hello_pcm.h"
//#include "sound4p.h"
//#include "AUD_8000_CH1.h"
//#include "AUD_8000_CH1_BE.h"
#include "AUD_8000_CH2_BE.h"
//#include "g711_8k_16bit.h"

AudioPool audpool;

void audtx11_irq(void *ctx)
{
	audpool.audtx_now_pos = I2sTxPos();
//	printf("AUDTX: 0x%08X\n", pos);
	audpool.audtx_irq_count++;

	UINT size = AUD_8000_CH2_BE_LEN;
	if (audpool.audtx_now_pos >= ((intptr_t)AUD_8000_CH2_BE) + size) {
		I2S_TXLR = 0;
		I2sSetTxEn(ENX_OFF);
		I2sSetTxEn(ENX_ON);
		I2S_TXLR = 3;
	}
}

void audrx11_irq(void *ctx)
{
	audpool.audrx_now_pos = I2sRxPos();
//	printf("AUDRX: 0x%08X\n", audpool.audrx_now_pos);
	audpool.audrx_irq_count++;
}

void audtx_irq(void *ctx)
{
	audpool.audtx_now_pos = I2sTxPos();
//	printf("AUDTX: 0x%08X\n", audpool.audtx_now_pos);
	audpool.audtx_irq_count++;

#if 0
	static UINT oldpos = 0;
	int getsize = pos - oldpos;

	int length = audpool.length + getsize;
	if (length >= AUD_STM_SIZE) {
		audpool.length = AUD_STM_SIZE;
		audpool.over_length = length - AUD_STM_SIZE;
		audpool.over_strm_base = audpool.strm_base + audpool.length;
		audpool.flag = 1;
	} else if (audpool.length == 0) {
		audpool.length = length;
		audpool.over_length = 0;
		audpool.strm_base = pos - length;
	} else {
		audpool.length += AUD_G711_SIZE;
		audpool.over_length = 0;
	}
	audpool.cnt++;

	if (audpool.flag == 1) {
		UINT ts = audpool.length;

		if (MsgStmPut(audpool.strm_base, audpool.length, ts, eSTREAMMSG_AUDIO) == ENX_OK) {
			IsrStreamdata(ctx);
		} else {
			printf("%s Drop\n", __func__);
		}
		//MsgRecPut(audpool.strm_base, audpool.length, ts, eSTREAMMSG_AUDIO);
		//IsrRecorddata(ctx);

		// audpool reset
		audpool.strm_base = audpool.over_strm_base;
		audpool.length = audpool.over_length;
		audpool.flag = 0;
		audpool.over_length = 0;
		audpool.over_strm_base = 0;
	}

	oldpos = pos;

	UINT size = G711DATA_LEN;
	if (pos >= ((intptr_t)g711data) + size) {
		I2S_TXLR = 0;
		I2sSetTxEn(ENX_OFF);
		oldpos = (intptr_t)g711data;
		audpool.strm_base = oldpos;
		I2sSetTxEn(ENX_ON);
		I2S_TXLR = 3;
	}
#elif 0
	UINT size = G711DATA_LEN;
	if (pos >= ((intptr_t)g711data) + size) {
		I2S_TXLR = 0;
		I2sSetTxEn(ENX_OFF);
		I2sSetTxEn(ENX_ON);
		I2S_TXLR = 3;
	}
#endif
}

UINT audrx_queue_put(UINT length, UINT *addr)
{
	while (length >= AUD_STM_SIZE) {
#if (AUDIO_CODEC==AUDIO_CODEC_RAW)
		UINT getts = AUD_STM_SIZE >> 1;
#else
		UINT getts = AUD_STM_SIZE;
#endif
		//printf("\tAUDIO PUT 0x%08X~0x%08X, %4u/%4u\n", *addr, *addr + AUD_STM_SIZE, AUD_STM_SIZE, length);
		if (MsgStmPut(*addr, AUD_STM_SIZE, getts, eSTREAMMSG_AUDIO) == ENX_OK) {
			IsrStreamdata();
		} else {
			printf("%s(STM) Drop\n", __func__);
		}

		if (MsgRecPut(*addr, AUD_STM_SIZE, getts, eSTREAMMSG_AUDIO) == ENX_OK) {
			IsrRecorddata();
		} else {
			printf("%s(REC) Drop\n", __func__);
		}

		length -= AUD_STM_SIZE;
		*addr += AUD_STM_SIZE;
	}
	//printf("\tAUDIO Remain                          %4u\n", length);
	return length;
}

void audrx_irq(void *ctx)
{
	audpool.audrx_now_pos = I2sRxPos();
//	printf("AUDRX: 0x%08X\n", audpool.audrx_now_pos);
	audpool.audrx_irq_count++;

#if 1
#if 1
#if 1
#if 1
	int getsize;
	int loop = 0;
	if (audpool.audrx_now_pos < audpool.audrx_old_pos) {
		loop = 1;
		getsize = (UINT)(intptr_t)audpool.audrx_buffer + audpool.audrx_buffer_length - audpool.audrx_old_pos;
		getsize += audpool.audrx_now_pos - (UINT)(intptr_t)audpool.audrx_buffer;
//		_Yprintf("New:0x%08X Old:0x%08X %uByte - ", audpool.audrx_now_pos, audpool.audrx_old_pos, getsize);
//		_Rprintf("X\n");
	} else {
		getsize = audpool.audrx_now_pos - audpool.audrx_old_pos;
//		_Yprintf("New:0x%08X Old:0x%08X %uByte - ", audpool.audrx_now_pos, audpool.audrx_old_pos, getsize);
//		_Gprintf("O\n");
	}

	if (loop == 1) {
		// 버퍼 꼬리에 남은 데이터의 총량 계산(size)
		int tailsize = (UINT)(intptr_t)audpool.audrx_buffer + audpool.audrx_buffer_length - audpool.audrx_old_pos;

		// if size > AUD_STM_SIZE
		//    최대한 queue에 put
		int remain_size = audrx_queue_put(tailsize, &audpool.audrx_old_pos);

		// 총 size를 다시 계산
		getsize -= (tailsize - remain_size);

		// 잔여 size를 buffer의 앞으로 복사. old pos 위치 이동
		if (remain_size) {
//			_Cprintf("\tLoop Copy: 0x%08X => 0x%08X, %dbyte\n", audpool.audrx_old_pos, audpool.audrx_buffer - remain_size, remain_size);
			BDmaMemCpy_isr(0, audpool.audrx_buffer - remain_size, (BYTE *)(intptr_t)audpool.audrx_old_pos, remain_size);
		} else {
//			_Cprintf("\tLoop Copy: 0x%08X => 0x%08X, ZERO\n", audpool.audrx_old_pos, audpool.audrx_buffer);
		}

		// old pos의 위치를 이동
		audpool.audrx_old_pos = (UINT)(intptr_t)audpool.audrx_buffer - remain_size;
	}

	audrx_queue_put(getsize, &audpool.audrx_old_pos);

//	printf("\n");

#else
	int getsize;
	_Yprintf("POS:0x%08X Old:0x%08X - %d - ", audpool.audrx_now_pos, audpool.audrx_old_pos, audpool.audrx_now_pos - audpool.audrx_old_pos);
	int loop = 0;
	if (audpool.audrx_now_pos < audpool.audrx_old_pos) {
		loop = 1;
		getsize = audpool.audrx_buffer + audpool.audrx_buffer_length - audpool.audrx_old_pos;
		//_Rprintf("Audio-Rx Buffer loop! - %d -", getsize);
	} else {
		getsize = audpool.audrx_now_pos - audpool.audrx_old_pos;
	}

	int length = audpool.length + getsize;
	if (length >= AUD_STM_SIZE) {
		if (loop == 1){
			_Rprintf("case1(0x%08X~0x%08X/%3d/%3d)", audpool.strm_base, audpool.strm_base + AUD_STM_SIZE, audpool.length, length);
		} else {
			//_Gprintf("case1");
		}
		audpool.length = AUD_STM_SIZE;
		audpool.over_length = length - AUD_STM_SIZE;
		audpool.over_strm_base = audpool.strm_base + audpool.length;
		audpool.flag = 1;
	} else if (audpool.length == 0) {
		if (loop == 1){
			_Rprintf("case2(0x%08X~0x%08X/%3d/%3d)", audpool.strm_base, audpool.strm_base + AUD_STM_SIZE, audpool.length, length);
		} else {
			//_Gprintf("case2");
		}
		audpool.length = length;
		audpool.over_length = 0;
		audpool.strm_base = audpool.audrx_now_pos - length;
	} else {
		if (loop == 1){
//			_Rprintf("case3(%d/%d) - strm_base(0x%08X), end_addr(0x%08X)", audrx_remain_size, audpool.length, audpool.strm_base, audrx_end_addr);
			_Rprintf("case3(0x%08X->0x%08X/%3d/%3d)", audpool.strm_base, audpool.audrx_buffer - audpool.length, audpool.length, length);
			BDmaMemCpy_isr(0, audpool.audrx_buffer - audpool.length, audpool.strm_base, audpool.length);
			audpool.strm_base = audpool.audrx_buffer - audpool.length;
		} else {
			//_Gprintf("case3");
		}
		audpool.length += getsize;
		audpool.over_length = 0;
	}
	audpool.cnt++;

	if (audpool.flag == 1) {
		printf(" - Audio Put, 0x%08X~0x%08X, %ubyte", audpool.strm_base, audpool.strm_base+audpool.length, audpool.length);
#if (AUDIO_CODEC==AUDIO_CODEC_RAW)
		UINT getts = audpool.length >> 1;
#else
		UINT getts = audpool.length;
#endif

		if (MsgStmPut(audpool.strm_base, audpool.length, getts, eSTREAMMSG_AUDIO) == ENX_OK) {
			IsrStreamdata(ctx);
		} else {
			printf("%s Drop\n", __func__);
		}

		// audpool reset
		audpool.strm_base = audpool.over_strm_base;
		audpool.length = audpool.over_length;
		audpool.flag = 0;
		audpool.over_length = 0;
		audpool.over_strm_base = 0;
	}
	printf("\n");
	audpool.audrx_old_pos = audpool.audrx_now_pos;
#endif
#else
	int getsize;
	//_Yprintf("POS:0x%08X Old:0x%08X - %d\n", audpool.audrx_now_pos, audpool.audrx_old_pos, audpool.audrx_old_pos - audpool.audrx_now_pos);
	if (audpool.audrx_now_pos < audpool.audrx_old_pos) {
		getsize = audpool.audrx_buffer + audpool.audrx_buffer_length - audpool.audrx_old_pos;
		//_Rprintf("Audio-Rx Buffer loop! - %d\n", getsize);
	} else {
		getsize = audpool.audrx_now_pos - audpool.audrx_old_pos;
	}

#if (AUDIO_CODEC==AUDIO_CODEC_RAW)
	UINT getts = getsize >> 1;
#else
	UINT getts = getsize;
#endif

	if (MsgStmPut(audpool.audrx_old_pos, getsize, getts, eSTREAMMSG_AUDIO) == ENX_OK) {
		IsrStreamdata(ctx);
	} else {
		printf("%s(STM) Drop\n", __func__);
	}

	if (MsgRecPut(audpool.audrx_old_pos, getsize, getts, eSTREAMMSG_AUDIO) == ENX_OK) {
		IsrRecorddata(ctx);
	} else {
		printf("%s(REC) Drop\n", __func__);
	}

	audpool.audrx_old_pos = audpool.audrx_now_pos;
#endif
#else
	int getsize;
	if (audpool.audrx_now_pos < audpool.audrx_old_pos) {
		getsize = audpool.audrx_buffer + audpool.audrx_buffer_length - audpool.audrx_old_pos;
	} else {
		getsize = audpool.audrx_now_pos - audpool.audrx_old_pos; // 오류 발생!!!!!!! 주소 overflow
	}

	printf("Get Pos: 0x%08X / oldPos: 0x%08X / Size: %u / length: %u / ",
			audpool.audrx_now_pos, audpool.audrx_old_pos, getsize, audpool.length + getsize);

	int length = audpool.length + getsize;
	if (length >= AUD_STM_SIZE) {
		_Gprintf("case1\n");
		audpool.length = AUD_STM_SIZE;
		audpool.over_length = length - AUD_STM_SIZE;
		audpool.over_strm_base = audpool.strm_base + audpool.length;
		audpool.flag = 1;
	} else if (audpool.length == 0) {
		_Gprintf("case2\n");
		audpool.length = length;
		audpool.over_length = 0;
		audpool.strm_base = audpool.audrx_now_pos - length;
	} else {
		_Gprintf("case3\n");
		audpool.length += getsize;
		audpool.over_length = 0;
	}
	audpool.cnt++;

	if (audpool.flag == 1) {
		printf("Audio Put, %ubyte\n", audpool.length);
		UINT ts = audpool.length;

		if (MsgStmPut(audpool.strm_base, audpool.length, ts, eSTREAMMSG_AUDIO) == ENX_OK) {
			IsrStreamdata(ctx);
		} else {
			printf("%s Drop\n", __func__);
		}
		//MsgRecPut(audpool.strm_base, audpool.length, ts, eSTREAMMSG_AUDIO);
		//IsrRecorddata(ctx);

		// audpool reset
		audpool.strm_base = audpool.over_strm_base;
		audpool.length = audpool.over_length;
		audpool.flag = 0;
		audpool.over_length = 0;
		audpool.over_strm_base = 0;
	}

	audpool.audrx_old_pos = audpool.audrx_now_pos;
#endif
#endif
}

void audtx_start(void)
{
#if 0
#if 1
	audpool.audtx_buffer = audpool.audtx_malloc = (BYTE *)AUD_8000_CH2_BE;
	audpool.audtx_buffer_length = audpool.audtx_malloc_length = AUD_8000_CH2_BE_LEN;
#else
	audpool.audtx_buffer = audpool.audtx_malloc = (BYTE *)g711data;
	audpool.audtx_buffer_length = audpool.audtx_malloc_length = G711DATA_LEN;
#endif
#else
	audpool.audtx_buffer = audpool.audtx_malloc = audpool.audrx_buffer;
	audpool.audtx_buffer_length = audpool.audtx_malloc_length = audpool.audrx_buffer_length;
#endif

	_Cprintf("Audio TX Base Address(0x%08X~0x%08X)\n", (intptr_t)audpool.audtx_buffer, audpool.audtx_buffer + audpool.audtx_buffer_length, audpool.audtx_buffer_length);
	I2sTxBaseAddr((BYTE *)audpool.audtx_buffer);

	I2sTxIrqCallback(audtx_irq, NULL);
	//I2sTxIrqCallback(audtx11_irq, NULL);
	I2sSetTxIrqEn(ENX_ON);

	I2sSetTxEn(ENX_ON);
}

void audrx_start(void)
{
	UINT rxbuf_size = 0;
	UINT wrlen = I2sGetWrLen();
	switch (wrlen) {
	case 0: // 128K
		rxbuf_size = 128;
		break;
	case 1: // 256K
		rxbuf_size = 256;
		break;
	case 2: // 512K
		rxbuf_size = 512;
		break;
	case 3: // 1024K
		rxbuf_size = 1024;
		break;
	default:
		_Rprintf("Error I2S WRLEN(%u)\n", wrlen);
		return;
	}

	audpool.audrx_buffer_length = rxbuf_size * ENX_AUDMEM_ALIGNMENT;
	audpool.audrx_malloc_length = audpool.audrx_buffer_length + (ENX_AUDMEM_ALIGNMENT * 2);

	audpool.audrx_malloc = pvPortMalloc(audpool.audrx_malloc_length);
	audpool.audrx_buffer = (BYTE *)ENX_AUDMEM_ALIGN(audpool.audrx_malloc) + ENX_AUDMEM_ALIGNMENT;
	_Cprintf("Audio RX AllocAddress(0x%08X~0x%08X) size(%u)\n", audpool.audrx_malloc, audpool.audrx_malloc + audpool.audrx_malloc_length, audpool.audrx_malloc_length);
	_Cprintf("Audio RX Base Address(0x%08X~0x%08X) size(%u)\n", audpool.audrx_buffer, audpool.audrx_buffer + audpool.audrx_buffer_length, audpool.audrx_buffer_length);

	//audpool.strm_base = (UINT)(intptr_t)audpool.audrx_buffer;
	audpool.audrx_old_pos = (UINT)(intptr_t)audpool.audrx_buffer;

	I2sRxBaseAddr((BYTE *)audpool.audrx_buffer);
	//I2sRxBaseAddr(0x80C00000);
	I2sRxIrqCallback(audrx_irq, NULL);
	//I2sRxIrqCallback(audrx11_irq, NULL);
	I2sSetRxIrqEn(ENX_ON);

	I2sSetRxEn(ENX_ON);
}

void audtxrx_task(void* ctx)
{
#if 0 // g711data tx
	oldpos = (UINT)(intptr_t)g711data;
	audpool.strm_base = (UINT)(intptr_t)g711data;
	audpool.aud_strm_base = audpool.strm_base;
#endif

	vTaskDelay(500);

	audrx_start();
	vTaskDelay(100);
	audtx_start();

	vTaskDelete(NULL);
}
#endif
