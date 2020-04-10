
//#include "../../main.h"
#include "dev.h"
#include "TP_main.h"

static int video_format = TP2802_720P30;

static const unsigned char CLK_MODE[4] = { 0x01,0x10,0x01,0x10 }; //for SDR_1CH/DDR_1CH output only
static const unsigned char CLK_ADDR[4] = { 0xfa,0xfa,0xfb,0xfb };
static const unsigned char CLK_AND[4] = { 0xf8,0x8f,0xf8,0x8f };
static const unsigned char SYS_AND[5] = { 0xfe,0xfd,0xfb,0xf7,0xf0 };
static const unsigned char SYS_MODE[5] = { 0x01,0x02,0x04,0x08,0x0f }; //{ch1,ch2,ch3,ch4,ch_all}
static unsigned char tp2827c_i2c_addr[] = { 
	TP2802A_I2C_ADDR, TP2802B_I2C_ADDR, TP2802C_I2C_ADDR, TP2802D_I2C_ADDR 
};

enum{
	N3_NO_ERROR,
	N3_WRITE_NOACK1,
	N3_WRITE_NOACK2,
	N3_READ_NOACK1,
	N3_READ_NOACK2,
};


void NXT_I2C_Write(BYTE AhdDevAddr, BYTE AhdRegAddr, BYTE AhdRegData)
{

	while(I2cWrite(1, AhdDevAddr,0,0));// Avg. 47 us (I2C_CLK = 290KHz)

	I2C_CODE(N3_WRITE_NOACK1, I2cWrite(1, AhdRegAddr, 0,0));
	I2C_CODE(N3_WRITE_NOACK2, I2cWrite(1, AhdRegData, 1,0));

	return;
}

void NXT_I2C_Read(BYTE AhdDevAddr, BYTE AhdRegAddr, BYTE* AhdRegData)
{
	BYTE bData;

	while(I2cWrite(1,AhdDevAddr,0,0));
	I2C_CODE(N3_READ_NOACK1, I2cWrite(1,AhdRegAddr, 0,1));
	I2C_CODE(N3_READ_NOACK2, I2cWrite(1,AhdDevAddr | 0x1, 0,0));

	bData = I2cRead(1,1,0);
	AhdRegData[0] = bData;

	return;
}


static void tp28xx_byte_write(unsigned char chip, unsigned char addr, unsigned char data)
{
	unsigned char chip_addr = tp2827c_i2c_addr[chip];
	NXT_I2C_Write(chip_addr, addr, data);

}

static unsigned char tp28xx_byte_read(unsigned char chip, unsigned char addr)
{
	unsigned char chip_addr = tp2827c_i2c_addr[chip];
	BYTE val = 0;
	//unsigned char val = 0;
	NXT_I2C_Read(chip_addr, addr, &val);
	return val;
}

static void tp2827c_write_table(unsigned char chip, unsigned char addr, unsigned char *tbl_ptr, unsigned char tbl_cnt)
{
	unsigned char i = 0;
	for (i = 0; i < tbl_cnt; i++) {
		tp28xx_byte_write(chip, (addr + i), *(tbl_ptr + i));
	}
}

static void tp2827c_set_reg_page(unsigned char chip, unsigned char ch)
{
	switch (ch)
	{
	case CH_1:		tp28xx_byte_write(chip, 0x40, 0x00);	break;  // VIN1 registers
	case CH_2:		tp28xx_byte_write(chip, 0x40, 0x01);	break;  // VIN2 registers
	case CH_3:		tp28xx_byte_write(chip, 0x40, 0x02);	break;  // VIN3 registers
	case CH_4:		tp28xx_byte_write(chip, 0x40, 0x03);	break;  // VIN4 registers
	case CH_ALL:	tp28xx_byte_write(chip, 0x40, 0x04);	break;  // Write All VIN1-4 registers
	case AUDIO_PAGE:	tp28xx_byte_write(chip, 0x40, 0x40);	break;  // Audio
	case DATA_PAGE:		tp28xx_byte_write(chip, 0x40, 0x10);	break;  // PTZ data
	default:		tp28xx_byte_write(chip, 0x40, 0x04);	break;
	}
}


static void tp2827c_set_work_mode_1080p30(unsigned chip)
{	// Start address 0x15, Size = 9B
	unsigned char tbl_tp2827c_1080p30_raster[] = { 0x03, 0xD3, 0x80, 0x29, 0x38, 0x47, 0x00, 0x08, 0x98 };
	tp2827c_write_table(chip, 0x15, tbl_tp2827c_1080p30_raster, 9);
}

static void tp2827c_set_work_mode_720p30(unsigned chip)
{	// Start address 0x15, Size = 9B
	unsigned char tbl_tp2827c_720p30_raster[] = { 0x13, 0x16, 0x00, 0x19, 0xD0, 0x25, 0x00, 0x0C, 0xE4 };
	tp2827c_write_table(chip, 0x15, tbl_tp2827c_720p30_raster, 9);
}

static void tp2827c_set_work_mode_720p60(unsigned chip)
{	// Start address 0x15, Size = 9B
	unsigned char tbl_tp2827c_720p60_raster[] = { 0x13, 0x16, 0x00, 0x19, 0xD0, 0x25, 0x00, 0x06, 0x72 };
	tp2827c_write_table(chip, 0x15, tbl_tp2827c_720p60_raster, 9);
}

static void TP2827C_PLL_Reset(int chip)
{
	int i, val;

	tp28xx_byte_write(chip, 0x45, 0xc9);

	for (i = 0; i < 6; i++)	{
		tp28xx_byte_write(chip, 0x44, 0x47);
		tp28xx_byte_write(chip, 0x42, 0x0C);
		tp28xx_byte_write(chip, 0x44, 0x07);
		tp28xx_byte_write(chip, 0x42, 0x00);
		//msleep(1);
		val = tp28xx_byte_read(chip, 0x01);
		if (0x08 != val) break;
	}
}

static void TP2827C_output(unsigned char chip)
{
	unsigned char tmp = 0;

	tp28xx_byte_write(chip, 0xF5, 0x00);
	tp28xx_byte_write(chip, 0xF1, 0x00);
	tp28xx_byte_write(chip, 0x4f, 0x03);

	// SDR_1CH
	tp28xx_byte_write(chip, 0xFA, 0x88);
	tp28xx_byte_write(chip, 0xFB, 0x88);
	tp28xx_byte_write(chip, 0xF4, 0x00); //
	tp28xx_byte_write(chip, 0xF6, 0x00);
	tp28xx_byte_write(chip, 0xF7, 0x11);
	tp28xx_byte_write(chip, 0xF8, 0x22);
	tp28xx_byte_write(chip, 0xF9, 0x33);
	tp28xx_byte_write(chip, 0x50, 0x00); //
	tp28xx_byte_write(chip, 0x51, 0x00); //
	tp28xx_byte_write(chip, 0x52, 0x00); //
	tp28xx_byte_write(chip, 0x53, 0x00); //
	tp28xx_byte_write(chip, 0xF3, 0x77);
	tp28xx_byte_write(chip, 0xF2, 0x77);

	tp28xx_byte_write(chip, 0x4D, 0x0f);
	tp28xx_byte_write(chip, 0x4E, 0x0f);

	if (TP2802_720P30V2 == video_format)	{
		tmp = tp28xx_byte_read(chip, 0xFA);
		tmp &= 0x88;
		tmp |= 0x11;
		tp28xx_byte_write(chip, 0xFA, tmp);
		tmp = tp28xx_byte_read(chip, 0xFB);
		tmp &= 0x88;
		tmp |= 0x11;
		tp28xx_byte_write(chip, 0xFB, tmp);
	}
}

static void TP2827C_reset_default(unsigned char chip)
{
	unsigned char tmp;

	tp28xx_byte_write(chip, 0x07, 0xc0);
	tp28xx_byte_write(chip, 0x0b, 0xc0);
	tp28xx_byte_write(chip, 0x21, 0x84);
	tp28xx_byte_write(chip, 0x38, 0x00);
	tp28xx_byte_write(chip, 0x39, 0x1C);
	tp28xx_byte_write(chip, 0x3a, 0x32);
	tp28xx_byte_write(chip, 0x3B, 0x26);

	tmp = tp28xx_byte_read(chip, 0x26);
	tmp &= 0xfe;
	tp28xx_byte_write(chip, 0x26, tmp);

	tmp = tp28xx_byte_read(chip, 0x06);
	tmp &= 0xfb;
	tp28xx_byte_write(chip, 0x06, tmp);
}

static void tp282x_SYSCLK_V1(unsigned char chip, unsigned char ch)
{
	unsigned char tmp, i;

	if (ch >= CH_ALL) {
		for (i = 0; i < 4; i++) { //four ports
			tmp = tp28xx_byte_read(chip, CLK_ADDR[i]);
			tmp &= CLK_AND[i];
			tp28xx_byte_write(chip, CLK_ADDR[i], tmp);
		}
	} else {
		tmp = tp28xx_byte_read(chip, CLK_ADDR[ch]);
		tmp &= CLK_AND[ch];
		tp28xx_byte_write(chip, CLK_ADDR[ch], tmp);
	}
}

static void tp282x_SYSCLK_V2(unsigned char chip, unsigned char ch)
{
	unsigned char tmp, i;

	if (ch >= CH_ALL) { //four ports
		for (i = 0; i < 4; i++) {
			tmp = tp28xx_byte_read(chip, CLK_ADDR[i]);
			tmp &= CLK_AND[i];
			tmp |= CLK_MODE[i];
			tp28xx_byte_write(chip, CLK_ADDR[i], tmp);
		}
	} else {
		tmp = tp28xx_byte_read(chip, CLK_ADDR[ch]);
		tmp &= CLK_AND[ch];
		tmp |= CLK_MODE[ch];
		tp28xx_byte_write(chip, CLK_ADDR[ch], tmp);
	}
}

static void tp282x_SYSCLK_V3(unsigned char chip, unsigned char ch)
{
	unsigned char tmp, i;

	tmp = tp28xx_byte_read(chip, 0x35);
	tmp |= 0x40;
	tp28xx_byte_write(chip, 0x35, tmp);

	if (ch >= CH_ALL) {
		for (i = 0; i < 4; i++) { //four ports
			tmp = tp28xx_byte_read(chip, CLK_ADDR[i]);
			tmp &= CLK_AND[i];
			tp28xx_byte_write(chip, CLK_ADDR[i], tmp);
			//tp28xx_byte_write(chip, DAT_ADDR[i], SDR1_SEL[i]);
		}
	} else {
		tmp = tp28xx_byte_read(chip, CLK_ADDR[ch]);
		tmp &= CLK_AND[ch];
		tp28xx_byte_write(chip, CLK_ADDR[ch], tmp);
		//tp28xx_byte_write(chip, DAT_ADDR[ch], SDR1_SEL[ch]);
	}
}

static void TP2827C_V1_DataSet(unsigned char chip)
{
	unsigned char tmp;

	tp28xx_byte_write(chip, 0x0b, 0xc0);
	tp28xx_byte_write(chip, 0x0c, 0x03);
	tp28xx_byte_write(chip, 0x0d, 0x50);

	tp28xx_byte_write(chip, 0x20, 0x30);
	tp28xx_byte_write(chip, 0x21, 0x84);
	tp28xx_byte_write(chip, 0x22, 0x36);
	tp28xx_byte_write(chip, 0x23, 0x3c);

	tp28xx_byte_write(chip, 0x25, 0xff);
	tp28xx_byte_write(chip, 0x26, 0x05);
	tp28xx_byte_write(chip, 0x27, 0x2d);
	tp28xx_byte_write(chip, 0x28, 0x00);

	tp28xx_byte_write(chip, 0x2b, 0x60);
	tp28xx_byte_write(chip, 0x2c, 0x0a);
	tp28xx_byte_write(chip, 0x2d, 0x30);
	tp28xx_byte_write(chip, 0x2e, 0x70);

	tp28xx_byte_write(chip, 0x30, 0x48);
	tp28xx_byte_write(chip, 0x31, 0xbb);
	tp28xx_byte_write(chip, 0x32, 0x2e);
	tp28xx_byte_write(chip, 0x33, 0x90);

	tp28xx_byte_write(chip, 0x38, 0x00);
	tp28xx_byte_write(chip, 0x39, 0x1c);
	tp28xx_byte_write(chip, 0x3a, 0x32);
	tp28xx_byte_write(chip, 0x3B, 0x26);

	tp28xx_byte_write(chip, 0x13, 0x00);
	tmp = tp28xx_byte_read(chip, 0x14);
	tmp &= 0x9f;
	tp28xx_byte_write(chip, 0x14, tmp);
}

static void TP2827C_V2_DataSet(unsigned char chip)
{
	unsigned char tmp;
	//tp28xx_byte_write(chip, 0x07, 0xc0);
	tp28xx_byte_write(chip, 0x0b, 0xc0);
	tp28xx_byte_write(chip, 0x0c, 0x13);
	tp28xx_byte_write(chip, 0x0d, 0x50);

	tp28xx_byte_write(chip, 0x20, 0x30);
	tp28xx_byte_write(chip, 0x21, 0x84);
	tp28xx_byte_write(chip, 0x22, 0x36);
	tp28xx_byte_write(chip, 0x23, 0x3c);

	tp28xx_byte_write(chip, 0x25, 0xff);
	tp28xx_byte_write(chip, 0x26, 0x05);
	tp28xx_byte_write(chip, 0x27, 0x2d);
	tp28xx_byte_write(chip, 0x28, 0x00);

	tp28xx_byte_write(chip, 0x2b, 0x60);
	tp28xx_byte_write(chip, 0x2c, 0x0a);
	tp28xx_byte_write(chip, 0x2d, 0x30);
	tp28xx_byte_write(chip, 0x2e, 0x70);

	tp28xx_byte_write(chip, 0x30, 0x48);
	tp28xx_byte_write(chip, 0x31, 0xbb);
	tp28xx_byte_write(chip, 0x32, 0x2e);
	tp28xx_byte_write(chip, 0x33, 0x90);
	//tp28xx_byte_write(chip, 0x35, 0x25);
	tp28xx_byte_write(chip, 0x38, 0x00);
	tp28xx_byte_write(chip, 0x39, 0x18);
	tp28xx_byte_write(chip, 0x3a, 0x32);
	tp28xx_byte_write(chip, 0x3B, 0x26);

	tp28xx_byte_write(chip, 0x13, 0x00);
	tmp = tp28xx_byte_read(chip, 0x14);
	tmp &= 0x9f;
	tp28xx_byte_write(chip, 0x14, tmp);
}

static void TP2827C_A720P30_DataSet(unsigned char chip)
{
	unsigned char tmp;
	tmp = tp28xx_byte_read(chip, 0x02);
	tmp |= 0x04;
	tp28xx_byte_write(chip, 0x02, tmp);

	tp28xx_byte_write(chip, 0x0d, 0x70);
	tp28xx_byte_write(chip, 0x20, 0x40);
	tp28xx_byte_write(chip, 0x21, 0x46);
	tp28xx_byte_write(chip, 0x25, 0xfe);
	tp28xx_byte_write(chip, 0x26, 0x01);
	tp28xx_byte_write(chip, 0x2c, 0x3a);
	tp28xx_byte_write(chip, 0x2d, 0x5a);
	tp28xx_byte_write(chip, 0x2e, 0x40);
	tp28xx_byte_write(chip, 0x30, 0x9d);
	tp28xx_byte_write(chip, 0x31, 0xca);
	tp28xx_byte_write(chip, 0x32, 0x01);
	tp28xx_byte_write(chip, 0x33, 0xd0);
}

static void TP2827C_A720P60_DataSet(unsigned char chip)
{
	unsigned char tmp;

	tmp = tp28xx_byte_read(chip, 0x14);
	tmp |= 0x60;
	tp28xx_byte_write(chip, 0x14, tmp);

	tp28xx_byte_write(chip, 0x20, 0x38);
	tp28xx_byte_write(chip, 0x21, 0x46);
	tp28xx_byte_write(chip, 0x25, 0xfe);
	tp28xx_byte_write(chip, 0x26, 0x01);
	tp28xx_byte_write(chip, 0x2d, 0x44);
	tp28xx_byte_write(chip, 0x2e, 0x40);
	tp28xx_byte_write(chip, 0x30, 0x29);
	tp28xx_byte_write(chip, 0x31, 0x62);
	tp28xx_byte_write(chip, 0x32, 0x78);
	tp28xx_byte_write(chip, 0x33, 0x10);
}
///////////////////////////////////////////////////////////////////
static void TP2827C_A1080P30_DataSet(unsigned char chip)
{
	unsigned char tmp;
	tmp = tp28xx_byte_read(chip, 0x02);
	tmp |= 0x04;
	tp28xx_byte_write(chip, 0x02, tmp);

	tp28xx_byte_write(chip, 0x15, 0x01);
	tp28xx_byte_write(chip, 0x16, 0xf0);

	tp28xx_byte_write(chip, 0x0d, 0x72);

	tp28xx_byte_write(chip, 0x20, 0x38);
	tp28xx_byte_write(chip, 0x21, 0x46);
	tp28xx_byte_write(chip, 0x25, 0xfe);
	tp28xx_byte_write(chip, 0x26, 0x0d);
	tp28xx_byte_write(chip, 0x2c, 0x3a);
	tp28xx_byte_write(chip, 0x2d, 0x54);
	tp28xx_byte_write(chip, 0x2e, 0x40);
	tp28xx_byte_write(chip, 0x30, 0xa5);
	tp28xx_byte_write(chip, 0x31, 0x95);
	tp28xx_byte_write(chip, 0x32, 0xe0);
	tp28xx_byte_write(chip, 0x33, 0x60);
}

static void TP2827C_1080P60_DataSet(unsigned char chip)
{
	unsigned char tmp;

	//tp28xx_byte_write(chip, 0x07, 0xc0);
	tp28xx_byte_write(chip, 0x0b, 0xc0);
	tp28xx_byte_write(chip, 0x0c, 0x03);
	tp28xx_byte_write(chip, 0x0d, 0x50);

	tp28xx_byte_write(chip, 0x15, 0x03);
	tp28xx_byte_write(chip, 0x16, 0xf0);
	tp28xx_byte_write(chip, 0x17, 0x80);
	tp28xx_byte_write(chip, 0x18, 0x12);
	tp28xx_byte_write(chip, 0x19, 0x38);
	tp28xx_byte_write(chip, 0x1a, 0x47);
	tp28xx_byte_write(chip, 0x1c, 0x08);
	tp28xx_byte_write(chip, 0x1d, 0x96);

	tp28xx_byte_write(chip, 0x20, 0x50);
	tp28xx_byte_write(chip, 0x21, 0x84);
	tp28xx_byte_write(chip, 0x22, 0x36);
	tp28xx_byte_write(chip, 0x23, 0x3c);

	tp28xx_byte_write(chip, 0x25, 0xff);
	tp28xx_byte_write(chip, 0x26, 0x05);
	tp28xx_byte_write(chip, 0x27, 0xad);
	tp28xx_byte_write(chip, 0x28, 0x00);

	tp28xx_byte_write(chip, 0x2b, 0x60);
	tp28xx_byte_write(chip, 0x2c, 0x0a);
	tp28xx_byte_write(chip, 0x2d, 0x40);
	tp28xx_byte_write(chip, 0x2e, 0x70);

	tp28xx_byte_write(chip, 0x30, 0x74);
	tp28xx_byte_write(chip, 0x31, 0x9b);
	tp28xx_byte_write(chip, 0x32, 0xa5);
	tp28xx_byte_write(chip, 0x33, 0xe0);

	//tp28xx_byte_write(chip, 0x35, 0x05);
	tp28xx_byte_write(chip, 0x38, 0x40);
	tp28xx_byte_write(chip, 0x39, 0x48);
	tp28xx_byte_write(chip, 0x3a, 0x12);
	tp28xx_byte_write(chip, 0x3b, 0x26);

	tp28xx_byte_write(chip, 0x13, 0x00);
	tmp = tp28xx_byte_read(chip, 0x14);
	tmp &= 0x9f;
	tp28xx_byte_write(chip, 0x14, tmp);
}

static int tp2827c_set_video_mode(unsigned char chip, unsigned char mode, unsigned char ch, unsigned char std)
{
	int err = 0;
	unsigned char tmp;

#if 1
	printf("[chip %d ch %2d] output mode SDR_1CH, set video mode to %#2x (", chip, ch, mode);

	switch (std) {
	case STD_TVI: printf("T");	break;
	case STD_HDA: printf("A");	break;
	case STD_HDC: printf("C");	break;
	default: printf("NG"); break;
	}

	switch (mode) {
	case TP2802_1080P60:	printf(" 1080P60		)\n");	break;
	case TP2802_1080P30:	printf(" 1080P30		)\n");	break;
	case TP2802_720P60:		printf(" 720P60			)\n");	break;
	case TP2802_720P30:		printf(" 720P30			)\n");	break;
	case TP2802_720P30V2:	printf(" 720P30V2		)\n");	break;
	}
#else
	(void) std;
#endif

	// Set Page Register to the appropriate Channel
	tp2827c_set_reg_page(chip, ch);

	switch (mode)
	{
	case TP2802_1080P60:
		tp28xx_byte_write(chip, 0x35, 0x05);
		tmp = tp28xx_byte_read(chip, 0x02);
		tmp &= 0xF8;
		tp28xx_byte_write(chip, 0x02, tmp);
		tmp = tp28xx_byte_read(chip, 0xf5);
		tmp &= SYS_AND[ch];
		tp28xx_byte_write(chip, 0xf5, tmp);
		TP2827C_1080P60_DataSet(chip);
		tp282x_SYSCLK_V3(chip, ch);
		break;
	case TP2802_1080P30:
		tp28xx_byte_write(chip, 0x35, 0x05);
		tp2827c_set_work_mode_1080p30(chip);
		tmp = tp28xx_byte_read(chip, 0x02);
		tmp &= 0xF8;
		tp28xx_byte_write(chip, 0x02, tmp);
		tmp = tp28xx_byte_read(chip, 0xf5);
		tmp &= SYS_AND[ch];
		tp28xx_byte_write(chip, 0xf5, tmp);
		TP2827C_V1_DataSet(chip);
		TP2827C_A1080P30_DataSet(chip);
		break;

	case TP2802_720P30:		// tvi
		tp28xx_byte_write(chip, 0x35, 0x05);
		tp2827c_set_work_mode_720p30(chip);
		tmp = tp28xx_byte_read(chip, 0x02);
		tmp &= 0xF8;
		tmp |= 0x02;
		tp28xx_byte_write(chip, 0x02, tmp);
		tmp = tp28xx_byte_read(chip, 0xf5);
		tmp &= SYS_AND[ch];
		tp28xx_byte_write(chip, 0xf5, tmp);
		
		TP2827C_V1_DataSet(chip);
		break;
	
	case TP2802_720P30V2:	// ahd
		tp28xx_byte_write(chip, 0x35, 0x25);
		tp2827c_set_work_mode_720p60(chip);
		tmp = tp28xx_byte_read(chip, 0x02);
		tmp &= 0xF8;
		tmp |= 0x02;
		tp28xx_byte_write(chip, 0x02, tmp);
		tmp = tp28xx_byte_read(chip, 0xf5);
		tmp |= SYS_MODE[ch];
		tp28xx_byte_write(chip, 0xf5, tmp);
		
		TP2827C_V2_DataSet(chip);
		TP2827C_A720P30_DataSet(chip);
		tp282x_SYSCLK_V2(chip, ch);
		break;

	case TP2802_720P60:
		tp28xx_byte_write(chip, 0x35, 0x05);
		tp2827c_set_work_mode_720p60(chip);
		tmp = tp28xx_byte_read(chip, 0x02);
		tmp &= 0xF8;
		tmp |= 0x02;
		tp28xx_byte_write(chip, 0x02, tmp);
		tmp = tp28xx_byte_read(chip, 0xf5);
		tmp &= SYS_AND[ch];
		tp28xx_byte_write(chip, 0xf5, tmp);
		
		TP2827C_V1_DataSet(chip);
		TP2827C_A720P60_DataSet(chip);
		break;

	default:
		err = -1;
		break;
	}

	if(err == 0) { tp282x_SYSCLK_V1(chip, ch); }

	return err;
}

///////////////////////////////////////////////////////////////
void TVI_Init(void)
{
	unsigned char chip = 0;
	unsigned char val = 0;

	//page reset
	tp28xx_byte_write(chip, 0x40, 0x00);

	// chip detect
	val = tp28xx_byte_read(chip, 0xfe);
	if (0x28 == val) { printf("Detected TP28xx\r\n"); }
	else 			 { printf("Invalid chip\r\n"); }
		
	TP2827C_PLL_Reset(chip);
	tp2827c_set_reg_page(chip, DEFAULT_CHANNEL);
	TP2827C_reset_default(chip);
	TP2827C_output(chip);
	// SDR_1CH
	tp28xx_byte_write(chip, 0x02, 0xC0 | BT656_HEADER_8BIT); //BT1120/BT656 header
	// to default preset.
	tp2827c_set_video_mode(chip, video_format, DEFAULT_CHANNEL, STD_TVI);
	// to set intended resolution.
	tp2827c_set_video_mode(chip, video_format, DEFAULT_CHANNEL, STD_HDA); // AHD 720 30FPS Initial
}


