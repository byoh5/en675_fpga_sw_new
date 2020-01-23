#include "dev.h"

#ifdef __EEPROM__

#define	TWI_EEP_PAGE_EA			256										// 24C64 PAGE No.
#define	TWI_EEP_BYTE_EA			32										// 24C64 BYTE No. each Page

#define ENABLE_I2C_ERR 0


#if model_I2cSlave == 2
	extern volatile int giI2cMasterOn;
	extern BYTE I2c_mode(const BYTE abMasterOn);

	#define I2C_MASTER_ON(ERR)	I2C_CODE(ERR, I2c_mode(1))
	#define I2C_MASTER_OFF()	I2c_mode(0);
#else
	#define I2c_mode(...)
	#define I2C_MASTER_ON(ERR)
	#define I2C_MASTER_OFF()
#endif

#define i2c_write(DAT,LAST,REPEAT)	I2cWrite(EEPROM_I2C_CH,DAT,LAST,REPEAT)
#define i2c_read(LAST,REPEAT)		I2cRead(EEPROM_I2C_CH,LAST,REPEAT)

BYTE gbEepromOff = 1;


BYTE EepWrite(const WORD awAddr, const BYTE abData)	// 201 us
{
	if(gbEepromOff) return EEP_WRITE_NODEV;
	I2C_STA

	I2C_MASTER_ON(EEP_WRITE_MASTER)

	while(i2c_write(EEPROM_I2C_CA,0,0));	// Avg. 47 us (I2C_CLK = 290KHz)
	I2C_CODE(EEP_WRITE_NOACK1, i2c_write((awAddr&0x1f00)>>8, 0,0))
	I2C_CODE(EEP_WRITE_NOACK2, i2c_write((awAddr&0xff), 0,0))

	I2C_CODE(EEP_WRITE_NOACK3, i2c_write(abData, 1,0))

	I2C_MASTER_OFF()
	return EEP_NO_ERROR;

	I2C_END
}

BYTE EepRead(const WORD awAddr, volatile BYTE* abData, const UINT anDataEa)	// 248 us (anDataEa = 1, I2C_CLK = 290KHz)
{
	if(gbEepromOff) return EEP_READ_NODEV;
	I2C_STA

	I2C_MASTER_ON(EEP_READ_MASTER)

	while(i2c_write(EEPROM_I2C_CA,0,0));
	I2C_CODE(EEP_READ_NOACK1, i2c_write((awAddr&0x1f00)>>8, 0,0))
	I2C_CODE(EEP_READ_NOACK2, i2c_write((awAddr&0xff), 0,1))

	I2C_CODE(EEP_READ_NOACK3, i2c_write(EEPROM_I2C_CA | 0x1, 0,0))

	UINT i;
	for(i=1; i<anDataEa; i++)
	{
		*abData = i2c_read(0,0);
		abData++;
	}
	*abData = i2c_read(1,0);

	I2C_MASTER_OFF()
	return EEP_NO_ERROR;

	I2C_END
}

BYTE EepPageWrite(const WORD awWaddr, BYTE* apbBuf, const UINT anBufEa)
{
	// 24LC64 Page Write
	// The master transmits up to 31 additional bytes which are temporarily stored in the on-chip page buffer
	//  and will be written into memory after the master has transmitted a stop condition.
	// After receipt of each word, the 5 lower address pointer bits are internally incremented by 1.
	// If the master should transmit more than 32 bytes prior to generating the stop condition,
	//  the address counter will roll over and the previously received data will be overwritten.
	//
	// If an attempt is made to write to the array with the WP pin held high,
	//  the device will acknowledge the command but no write cycle will occur,
	//  no data will be written and the device will immediately accept a new command.

	if(gbEepromOff) return EEP_WRITE_NODEV;
	I2C_STA

	I2C_MASTER_ON(EEP_WRITE_MASTER)

	while(i2c_write(EEPROM_I2C_CA,0,0));
	I2C_CODE(EEP_WRITE_NOACK1, i2c_write((awWaddr&0x1f00)>>8, 0,0))
	I2C_CODE(EEP_WRITE_NOACK2, i2c_write((awWaddr&0xff), 0,0))

	UINT i;
	for(i=1; i<anBufEa; i++)
	{
		I2C_CODE(EEP_WRITE_NOACK3, i2c_write(*apbBuf, 0,0))
		apbBuf++;
	}

	I2C_CODE(EEP_WRITE_NOACK4, i2c_write(*apbBuf, 1,0))

	I2C_MASTER_OFF()
	return EEP_NO_ERROR;

	I2C_END
}

BYTE ISRT TwiWrEep2( WORD awWaddr, volatile BYTE* apbBuf, UINT anBufEa)
{	// I2C EEPROM Page(32byte) Write Function
	// CAUTION ! :
	//			1 page Tx per 2 frame , because EEP restart delay)
	//			Fuction recall delay = after (*apbBuf ea / TWI_EEP_BYTE_EA) frame
	// apwWaddr : Start address
	// * apbBuf : input data p
	// anBufEa	: input data ea of byte

	static WORD gwWaddr;
	static BYTE k,l;
	static BYTE* gpbBuf;
	static BYTE gbTwiCnt;
		   BYTE bTwiCnt;

	WORD 		wRegAddrBuf;
	//BYTE 		bRegAddr[2];
	//BYTE 		i,t;

	if (gbTwiCnt%2)	{ gbTwiCnt++; return 0; }					// 1 page Tx per 2 frame
	else 			{ bTwiCnt = gbTwiCnt>>1; }

	if (!bTwiCnt) {											// Burst Tx Ready
		if (!anBufEa)	return 0;
		else if ((awWaddr+anBufEa)>=(TWI_EEP_PAGE_EA*TWI_EEP_BYTE_EA)) {
			INIT_STR("WrEEP Size Over ! ");
			return 0;
		}
		else {
			gwWaddr = awWaddr;
			gpbBuf  = (BYTE*)apbBuf;
			k = anBufEa / TWI_EEP_BYTE_EA;
			l = anBufEa % TWI_EEP_BYTE_EA;

			gbTwiCnt++;
			return 0;
		}
	}
	else if (bTwiCnt>=k+2) {								// Status reset at Tx end
		INIT_STR_HEX("TwiWrEep2 Done ! ", gwWaddr, 5);

		gwWaddr = 0;
		gpbBuf  = 0;
		k = 0;
		l = 0;
		gbTwiCnt = 0;
		return 1;											// Tx end flag
	}
	else {
		wRegAddrBuf = (bTwiCnt-1) * TWI_EEP_BYTE_EA;

		EepPageWrite(gwWaddr + wRegAddrBuf, gpbBuf + wRegAddrBuf, (bTwiCnt<k+1) ? TWI_EEP_BYTE_EA : l);
		//TxStrDec2("EepPageWrite :", gwWaddr + wRegAddrBuf, 5, (bTwiCnt<k+1) ? TWI_EEP_BYTE_EA : l, 3);

		gbTwiCnt++;

		return 0;
	}
}

void EepromInit(void)
{
	if(I2cCheck(EEPROM_I2C_CH, EEPROM_I2C_CA)) {
		gbEepromOff = 0;
		printf("EEPROM Connected...");
#if 1
		const WORD awAddr = ((64*1024)>>3) - 1;
		volatile BYTE abData;
		EepRead(awAddr, &abData, 1);
		printf(" Boot count:%d", abData++);
		EepWrite(awAddr, abData);
#endif
	}
	else {
		printf("EEPROM Not Connected...");
	}
	printf("\n");
}


#endif

