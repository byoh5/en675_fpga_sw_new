#include "dev.h"

#ifdef __RTC_LOAD__
#ifdef __RTC_S35390A__
#include <string.h>
#include <time.h>
#include "s35390a.h"

#define I2C_LOCK	//while (CS_I2C2)
#define I2C_UNLOCK	//CS_I2C2 = 0
#define I2C_CHECK(addr)				I2cCheck(RTC_I2C_CH, addr)
#define I2C_WRITE(addr, reg, data)	I2cWrite(RTC_I2C_CH, addr, reg, data)
#define I2C_READ(addr, reg)			I2cRead(RTC_I2C_CH, addr, reg)

static s35390a_t s35390a;

const unsigned char byte_rev_table[256] = {
	0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
	0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
	0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
	0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
	0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
	0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
	0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
	0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
	0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
	0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
	0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
	0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
	0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
	0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
	0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
	0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
	0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
	0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
	0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
	0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
	0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
	0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
	0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
	0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
	0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
	0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
	0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
	0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
	0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
	0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
	0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
	0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};

static unsigned char bitrev8(BYTE byte)
{
	return byte_rev_table[byte];
}

static UINT s35390a_check(void)
{
	I2C_LOCK;
	UINT nCkechk = I2C_CHECK(S35390A_I2C_READ);
	I2C_UNLOCK;
	return nCkechk;
}

/**
	read regs from S3539A RTC

	@param cmd : number of register
	@param buf : to be read.
	@param len : length of buf

	@return :
		ENX_OK : success.
		ENX_FAIL : fail
*/
static ENX_OKFAIL 
s35390a_read_regs(BYTE cmd, BYTE *buf, BYTE len)
{
	BYTE i = 0;

	I2C_LOCK;

	/*slave & cmd & rd operation */
	while(I2C_WRITE(S35390A_I2C_READ | (cmd << 1), 0, 0));

	/* read data */
	for(i=0; i<len-1; i++)
	{
		buf[i] = I2C_READ(0, 0);
	}

	/*finish to read reg and generate stop signal */
	buf[i] = I2C_READ(1, 1);

	I2C_UNLOCK;

	return ENX_OK;

}

static ENX_OKFAIL 
s35390a_write_regs(BYTE cmd, BYTE *buf, BYTE len)
{
	BYTE i = 0;

	I2C_LOCK;

	/*slave & cmd & rd operation */
	while(I2C_WRITE(S35390A_I2C_WRITE | (cmd << 1), 0, 0));

	/* write data */
	for(i=0;i<len-1;i++)
	{
		I2C_WRITE(buf[i], 0, 0);
	}

	/*finish to read reg and generate stop signal */
	I2C_WRITE(buf[i], 1, 0);

	I2C_UNLOCK;

	return ENX_OK;
}

/**
	reset s35390a

	@return
		ENX_OK : OK
		ENX_FAIL : FAIL
*/
static ENX_OKFAIL 
s35390a_reset(void)
{
	BYTE buf[1];

	if(s35390a_read_regs(S35390A_CMD_STATUS1, buf, sizeof(buf)) == ENX_FAIL)
		return ENX_FAIL;

	if(!(buf[0] & (S35390A_FLAG_POC | S35390A_FLAG_BLD)))
		return ENX_OK;

	buf[0] |= (S35390A_FLAG_RESET | S35390A_FLAG_24H);
	buf[0] &= 0xf0;

	if(s35390a_write_regs(S35390A_CMD_STATUS1, buf, sizeof(buf)) == ENX_FAIL)
		return ENX_FAIL;
	
	return ENX_OK;
}

/**
	@return 
		ENX_OK : OK
		ENX_FAIL : FAIL
*/
static ENX_OKFAIL 
s35390a_disable_test_mode(void)
{
	BYTE buf[1];

	if(s35390a_read_regs(S35390A_CMD_STATUS2, buf, sizeof(buf)) == ENX_FAIL)
		return ENX_FAIL;

	if(!(buf[0] & S35390A_FLAG_TEST))
		return ENX_OK;

	buf[0] &= ~S35390A_FLAG_TEST;

	if(s35390a_write_regs(S35390A_CMD_STATUS2, buf, sizeof(buf)) == ENX_FAIL)
		return ENX_FAIL;

	return ENX_OK;
}

static char 
s35390a_hr2reg(int hour)
{
	if(s35390a.twentyfourhour)
		return bin2bcd(hour);

	if(hour < 12)
		return bin2bcd(hour);

	return 0x40 | bin2bcd(hour - 12);
}

static int 
s35390a_reg2hr(char reg)
{
	unsigned hour;

	if(s35390a.twentyfourhour)
		return bcd2bin(reg & 0x3f);

	hour = bcd2bin(reg & 0x3f);
	if(reg & 0x40)
		hour += 12;

	return hour;
}

static ENX_OKFAIL 
s35390a_get_datetime(struct tm *_tm)
{
	int i;
	BYTE buf[7];

	if(s35390a_read_regs(S35390A_CMD_TIME1, buf, sizeof(buf)) == ENX_FAIL)
		return ENX_FAIL;

	/* This chip returns the bits of each byte in reverse order */
	for(i=0;i<7;i++)
		buf[i] = bitrev8(buf[i]);

	_tm->tm_sec = bcd2bin(buf[S35390A_BYTE_SECS]);
	_tm->tm_min = bcd2bin(buf[S35390A_BYTE_MINS]);
	_tm->tm_hour = s35390a_reg2hr(buf[S35390A_BYTE_HOURS]);
	_tm->tm_wday = bcd2bin(buf[S35390A_BYTE_WDAY]);
	_tm->tm_mday = bcd2bin(buf[S35390A_BYTE_DAY]);
	_tm->tm_mon = bcd2bin(buf[S35390A_BYTE_MONTH]) - 1;
	_tm->tm_year = bcd2bin(buf[S35390A_BYTE_YEAR]) + 100; /*orgin 2000 */

//	printf("%s(%d) : sec(%d) min(%d) hour(%d) wday(%d) mday(%d) mon(%d) year(%d)\r\n", __func__, __LINE__, _tm->tm_sec, _tm->tm_min, _tm->tm_hour, _tm->tm_wday, _tm->tm_mday, _tm->tm_mon, _tm->tm_year);
	return rtc_valid_tm(_tm);
}

static ENX_OKFAIL 
s35390a_set_datetime(struct tm *_tm)
{
	int i;
	BYTE buf[7];
//	printf("%s(%d) : sec(%d) min(%d) hour(%d) wday(%d) mday(%d) mon(%d) year(%d)\r\n", __func__, __LINE__, _tm->tm_sec, _tm->tm_min, _tm->tm_hour, _tm->tm_wday, _tm->tm_mday, _tm->tm_mon, _tm->tm_year);

	if(rtc_valid_tm(_tm) == ENX_FAIL)
		return ENX_FAIL;

	buf[S35390A_BYTE_YEAR]  = bin2bcd(_tm->tm_year - 100);
	buf[S35390A_BYTE_MONTH] = bin2bcd(_tm->tm_mon + 1);
	buf[S35390A_BYTE_DAY]   = bin2bcd(_tm->tm_mday);
	buf[S35390A_BYTE_WDAY]  = bin2bcd(_tm->tm_wday);
	buf[S35390A_BYTE_HOURS] = s35390a_hr2reg(_tm->tm_hour);
	buf[S35390A_BYTE_MINS]  = bin2bcd(_tm->tm_min);
	buf[S35390A_BYTE_SECS]  = bin2bcd(_tm->tm_sec);

	/* This chip expects the bits of each byte to be in reverse order */
	for(i=0;i<7;i++)
		buf[i] = bitrev8(buf[i]);

	return s35390a_write_regs(S35390A_CMD_TIME1, buf, sizeof(buf));
}

/**
	register s35390 to rtc_device.
	and init s35390

	@return
		0 : ENX_FAIL
		1 : ENX_OK
*/
ENX_OKFAIL
s35390a_init(void)
{
	BYTE buf[1];
 	struct tm _tm;
	int get_res;

	if(s35390a_check() == ENX_FAIL) 
	{
		printf("  >>S35390A Not Connected...\r\n");
		goto err;
	}

	printf("  >>S35390A Connected...\r\n");

	if(s35390a_reset() == ENX_FAIL)
		goto err;

	if(s35390a_disable_test_mode() == ENX_FAIL)
		goto err;

	get_res = s35390a_read_regs(S35390A_CMD_STATUS1, buf, sizeof(buf));
	if(get_res == ENX_FAIL)
		goto err;

	if (buf[0] & S35390A_FLAG_24H)	s35390a.twentyfourhour = 1;
	else							s35390a.twentyfourhour = 0;

	get_res = s35390a_get_datetime(&_tm);

	if (get_res == ENX_FAIL)
	{
		printf("clock needs to be set\r\n");		
		return ENX_FAIL;
	}

	rtc_device.read_time = s35390a_get_datetime;
	rtc_device.set_time  = s35390a_set_datetime;
	strcpy(rtc_device.name, "S35390A");

	return ENX_OK;
err:
    printf("rtc init is error\r\n");
    return ENX_FAIL;
}
#endif
#endif
