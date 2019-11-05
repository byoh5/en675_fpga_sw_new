#include "dev.h"
#ifdef __AUDIO__
#ifdef __AUDIO_TLV320AIC3206__
#include "tlv320aic3206.h"
#endif

void audio_init(void)
{
	GpioSetOut(AUDIO_GPIO_RST, GPIO_OUT_LOW);
	WaitXms(100);
	GpioSetOut(AUDIO_GPIO_RST, GPIO_OUT_HI);

	//tx_mode : 0: L, 1: R, 2: L+R/2, 3: Stereo -> 데이터를 전송할 방향, 2는 한 word의 데이터를 읽은 후 2로 divide, 양방향으로 전송.
	//tx_cd : 0 or 1: PCM, 2: G711-a, 3: G711-u)
	//tx_dw : 0->8 , 1->16, 2->24, 3->32 : 하는 데이터 width
	//rd_byte : 0: 128B, 1: 256B, 2: 512B, 3: 1KB -> 한번 request에서 읽는 데이터 량
	//rd_dw : 0->8 , 1->16, 2->24, 3->32 : Rx의 데이터 width
	//rd_len : 0: 128KB, 1: 256KB, 2: 512KB, 3: 1MB -> Loop를 도는 최대 데이터 량
	//tx_lr : 0 : Mute(0), 1: Left, 2: Right, 3: Both -> TX할 때 mute 또는 unmute 선택

	// (UINT tx_mode, UINT tx_cd, UINT tx_dw, UINT rd_byte, UINT rd_dw, UINT rd_len, UINT tx_lr)
	//I2sTxCfg(3, 0, 0, 3, 1, 0, 3); // Stereo 44.1KHz PCM 8bit,

#if (AUDIO_CODEC==AUDIO_CODEC_G711U) // Mono 8KHz 16bit G.711-u
	I2sTxCfg(2, eI2sCodecG711u, eI2sWidth16b, eI2sReq256B, eI2sWidth8b, eI2sLoop128KB, 3);	// Mono 8KHz, G.711-u, 16bit, Rd:512B Buf:128KB
	//I2sTxCfg(3, eI2sCodecG711u, 1, eI2sReq256B, 0, eI2sLoop128KB, 3);	// Mono 8KHz, G.711-u, 16bit, Rd:512B Buf:128KB
	I2sRxCfg(2, eI2sCodecG711u, eI2sWidth16b, eI2sReq256B, eI2sWidth8b, eI2sLoop128KB);		// Mono 8KHz, G.711-u, 16bit, Wr:512B Buf:128KB
#endif
#if (AUDIO_CODEC==AUDIO_CODEC_RAW) // 16bit PCM
	I2sTxCfg(3, eI2sCodecPCM, eI2sWidth16b, eI2sReq1024B, eI2sWidth16b, eI2sLoop128KB, 3);	// Stereo 44.1KHz, 16bit, Rd:1024B Buf:128KB
	I2sRxCfg(3, eI2sCodecPCM, eI2sWidth16b, eI2sReq1024B, eI2sWidth16b, eI2sLoop128KB);		// Stereo 44.1KHz, 16bit, Wr:1024B Buf:128KB
#endif

#ifdef __AUDIO_TLV320AIC3206__
	tlv320aic3206_init();
#endif
}
#endif
