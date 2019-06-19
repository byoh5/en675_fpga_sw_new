#include "dev.h"

#ifdef __AUDIO__
#ifdef __AUDIO_TLV320AIC3206__
#include "tlv320aic3206.h"

typedef struct {
	ENX_SWITCH status;
	char volume;	
} tlv320aic3206_state;

static tlv320aic3206_state tlv320aic3206_info = {
	.status = ENX_OFF,
	.volume = 0x00,	
};

const BYTE TLV320AIC3206_SetupA[][2] =
{
	//	Software Reset
	{0x00, 0x00},	//	Select Page 0
	{0x01, 0x01},	//	Software Reset
	{0xFE, 0x01},	//	Delay 1ms

	{0x00, 0x01},	//	Select Page 1
	{0x01, 0x08},	//	Disable weak AVDD to DVDD connection
	{0x02, 0x00},	//	Enable Master Analog Power Control	- Very important
	{0x7B, 0x01},	//	REF charging time = 40ms
	{0xFE, 0x28},	//	Delay 40ms
	{0x7C, 0x06},	//	8/8 CP Sizing (Setup A), Div = 6, 333kHz
	{0x01, 0x0A},	//	CP powered, source = int 8MHz OSC
	{0x0A, 0x00},	//	Full chip CM = 0.9V (Setup A)
	{0x03, 0x00},	//	PTM_P3, High Performance (Setup A)
	{0x04, 0x00},	//	PTM_P3, High Performance (Setup A)


	//	Codec PLL Setting (MCLK 24.576MHz)
#if 0
	//	Fs 44.1KHz = 24576/2*1*7.35/1/16/128 = 44.1
	{0x00, 0x00},	//	00 Select Page 0
	{0x04, 0x03},	//	04 Clock Setting - Multiplexers
	{0x05, 0xA1},	//	05 PLL P&R : P = 0x02, R = 0x01
	{0x06, 0x07},	//	06 PLL J   : J = 0x07
	{0x07, 0x0E},	//	07 PLL D MSB : P MSB = 0x0E
	{0x08, 0xAD}, 	//	08 PLL D LSB : P LSB = 0xAC (3500d)
	{0xFE, 0x0A},	//	Delay 10ms

	//	DAC Setting
	{0x0b, 0x81},	//	11 NDAC = 0x01
	{0x0c, 0x90},	//	12 MDAC = 0x10 (16d)
	{0x0D, 0x00},	//	13 DAC DOSR MSB Over-sampling: DOSR MSB = 0x00
	{0x0E, 0x80},	//	14 DAC DOST LSB Over-sampling: DOSR LSB = 0x80 (128d)
	{0xFE, 0x01},	//	Delay 1ms

	//	ADC Setting
	{0x12, 0x81},	//	18 NADC = 0x01
	{0x13, 0x90},	//	19 MADC = 0x10 (16d)
	{0x14, 0x80},	//	20 ADC AOSR = 0x80 (128d)
	{0xFE, 0x01},	//	Delay 1ms

	//	Codec Interface
	{0x1B, 0x0C},	//	27 Codec Interface Control : EN673 Slave I2S + 16bit + BCLK out + WCLK out
	{0x1D, 0x05},	//	29 Codec Interface Control2 : BDIV_CLKIN = DAC_MOD_CLK
	{0x1E, 0x84},	//	30 BCLK_N_VAL : BCLK N-divider is powered up, BCLK divider N = 04h
	{0xFE, 0x20},	//	Delay 1ms
#else
	//	Fs 8KHz = 24576/2*1*8/3/32/128 = 8
	{0x00, 0x00},	//	00 Select Page 0
	{0x04, 0x03},	//	04 Clock Setting - Multiplexers
	{0x05, 0xA1},	//	05 PLL P&R : P = 0x02, R = 0x01
	{0x06, 0x08},	//	06 PLL J   : J = 0x08
	{0x07, 0x00},	//	07 PLL D MSB : P MSB = 0x00
	{0x08, 0x00}, 	//	08 PLL D LSB : P LSB = 0x00
	{0xFE, 0x0A},	//	Delay 10ms

	//	DAC Setting
	{0x0b, 0x83},	//	11 NDAC = 0x03
	{0x0c, 0xA0},	//	12 MDAC = 0x20 (32d)
	{0x0D, 0x00},	//	13 DAC DOSR MSB Over-sampling: DOSR MSB = 0x00
	{0x0E, 0x80},	//	14 DAC DOST LSB Over-sampling: DOSR LSB = 0x80 (128d)
	{0xFE, 0x01},	//	Delay 1ms

	//	ADC Setting
	{0x12, 0x83},	//	18 NADC = 0x03
	{0x13, 0xA0},	//	19 MADC = 0x20 (32d)
	{0x14, 0x80},	//	20 ADC AOSR = 0x80 (128d)
	{0xFE, 0x01},	//	Delay 1ms

	//	Codec Interface
	{0x1B, 0x0C},	//	27 Codec Interface Control : EN673 Slave I2S + 16bit + BCLK out + WCLK out
	{0x1D, 0x05},	//	29 Codec Interface Control2 : BDIV_CLKIN = DAC_MOD_CLK
	{0x1E, 0x84},	//	30 BCLK_N_VAL : BCLK N-divider is powered up, BCLK divider N = 04h
	{0xFE, 0x20},	//	Delay 1ms
#endif

	//	Signal Processing Settings
	{0x00, 0x00},	//	Select Page 0
//	{0x3c, 0x01},	//	Set the DAC Mode to PRB_P1 (Setup A)
	//Output Channel Configuration
	{0x00, 0x01},	//	Select Page 1
	{0x0c, 0x08},	//	Route LDAC to HPL
	{0x0d, 0x08},	//	Route RDAC to HPR
	{0x00, 0x00},	//	Select Page 0
	{0x3f, 0xd6},	//	Power up LDAC/RDAC
	{0x00, 0x01},	//	Select Page 1
	{0x7d, 0x13},	//	GCHP Mode, OC for all, HP Sizing (Setup A)
	{0x10, 0x00},	//	Unmute HPL driver, 0dB Gain (Setup A)
	{0x11, 0x00},	//	Unmute HPR driver, 0dB Gain (Setup A)
	{0x16, 0x32},
	{0x17, 0x32},
	{0x09, 0x30},	//	Power up HPL/HPR drivers
	{0xFE, 0x03},	//	Delay 3 millisecond   // f 30 02 xxxxx1xx # Wait for offset correction to finish
	{0x00, 0x00},	//	Select Page 0
	{0x40, 0x00},	//	Unmute LDAC/RDAC
	{0x41, 0xD8},
	{0x42, 0xD8},
	{0x00, 0x04},	// Page 0
};	

static UINT tlv320aic3206_check(void)
{
	return I2cCheck(AUDIO_I2C_CH, TLV320AIC3206_I2C_WRITE);
}

void tlv320aic3206_delay(UINT xms)
{
	WaitXms(xms);
}

static ENX_OKFAIL tlv320aic3206_write_reg(BYTE reg, BYTE data)
{
	while (I2cWrite(AUDIO_I2C_CH, TLV320AIC3206_I2C_WRITE, 0, 0));
	if (I2cWrite(AUDIO_I2C_CH, reg, 0, 0)) return ENX_FAIL;
	if (I2cWrite(AUDIO_I2C_CH, data, 1, 0)) return ENX_FAIL;
	return ENX_OK;
}

static ENX_OKFAIL tlv320aic3206_read_reg(BYTE reg, BYTE *data)
{
	while (I2cWrite(AUDIO_I2C_CH, TLV320AIC3206_I2C_WRITE, 0, 0));
	if (I2cWrite(AUDIO_I2C_CH, reg, 1, 1)) return ENX_FAIL;
	while (I2cWrite(AUDIO_I2C_CH,TLV320AIC3206_I2C_READ, 0, 0));
	*data = I2cRead(AUDIO_I2C_CH, 1, 1);
	return ENX_OK;
} 

void tlv320aic3206_init(void)
{
	if (tlv320aic3206_check() == ENX_FAIL) {
		_Rprintf("  >>TLV320AIC3206 Not Connected...\n");
	} else {
		tlv320aic3206_info.status = ENX_ON;
		_Gprintf("  >>TLV320AIC3206 Connected...\n");

		int size = sizeof(TLV320AIC3206_SetupA) / sizeof(TLV320AIC3206_SetupA[0]);
		for (int i = 0; i < size; i++) {
			if (TLV320AIC3206_SetupA[i][0] == 0xFE) {
				tlv320aic3206_delay((UINT)TLV320AIC3206_SetupA[i][1]);
			} else {
				tlv320aic3206_write_reg(TLV320AIC3206_SetupA[i][0],TLV320AIC3206_SetupA[i][1]);
			}

//			BYTE u8Read;
//			tlv320aic3206_read_reg(TLV320AIC3206_SetupA[i][0], &u8Read);
//			printf("Read Adr = %08x  Reg = %08x\n", TLV320AIC3206_SetupA[i][0], u8Read);
		}
	}
}
#endif
#endif
