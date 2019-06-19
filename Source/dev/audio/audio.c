#include "dev.h"
#ifdef __AUDIO__
#ifdef __AUDIO_TLV320AIC3206__
#include "tlv320aic3206.h"
#endif
//#include "hello_pcm.h"
#include "g711_8k_16bit.h"

void audio_tx_irq(void *ctx)
{
	UINT pos = I2sTxPos();
	//printf("pos = 0x%08X\n", pos);
	//UINT size = sizeof(PCM_HELLO) / sizeof(PCM_HELLO[0]);
	//if (pos >= ((UINT)PCM_HELLO) + size) {
	UINT size = G711DATA_LEN;
	if (pos >= ((intptr_t)g711data) + size) {
		I2sSetTxEn(ENX_OFF);
		I2sSetTxEn(ENX_ON);
	}
}

void audio_init(void)
{
	GpioSetOut(AUDIO_GPIO_RST, GPIO_OUT_LOW);
	WaitXms(100);
	GpioSetOut(AUDIO_GPIO_RST, GPIO_OUT_HI);

	//tx_mode : 0: L, 1: R, 2: L+R/2, 3: Stereo -> 데이터를 전송할 방향, 2는 한 word의 데이터를 읽은 후 2로 divide, 양방향으로 전송.
	//tx_cd : 0 or 1: PCM, 2: G711-a, 3: G711-u)
	//tx_dw : 0->8 , 1->16, 2->24, 3->32 : Tx의 데이터 width
	//rd_byte : 0: 128B, 1: 256B, 2: 512B, 3: 1KB -> 한번 request에서 읽는 데이터 량
	//rd_dw : 0->8 , 1->16, 2->24, 3->32 : Rx의 데이터 width
	//rd_len : 0: 128KB, 1: 256KB, 2: 512KB, 3: 1MB -> Loop를 도는 최대 데이터 량
	//tx_lr : 0 : Mute(0), 1: Left, 2: Right, 3: Both -> TX할 때 mute 또는 unmute 선택

	// (UINT tx_mode, UINT tx_cd, UINT tx_dw, UINT rd_byte, UINT rd_dw, UINT rd_len, UINT tx_lr)
	//I2sTxCfg(3, 0, 0, 3, 1, 0, 3); // Stereo 44.1KHz PCM 8bit,

	I2sTxCfg(0, 3, 1, 0, 1, 0, 1);

#ifdef __AUDIO_TLV320AIC3206__
	tlv320aic3206_init();
#endif
	//I2sTxBaseAddr(PCM_HELLO);
	I2sTxBaseAddr((BYTE *)g711data);
#if 1
	I2sTxIrqCallback(audio_tx_irq, NULL);
	I2sSetTxIrqEn(ENX_ON);

	I2sSetTxEn(ENX_ON);
#endif
}


#endif
