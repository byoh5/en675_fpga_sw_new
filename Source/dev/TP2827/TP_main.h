#ifndef __TP2827C_H__
#define __TP2827C_H__


#define BT656_HEADER_8BIT   0x08 //reg0x02 bit3 1=656,
#define DEFAULT_CHANNEL CH_2

#define TP2802A_I2C_ADDR 	0x88
#define TP2802B_I2C_ADDR 	0x8A
#define TP2802C_I2C_ADDR 	0x8C
#define TP2802D_I2C_ADDR 	0x8E

#define TP2827C 0x270C

enum{
TP2802_1080P25 =	    0x03,
TP2802_1080P30 =	    0x02,
TP2802_720P25  =	    0x05,
TP2802_720P30  =    	0x04,
TP2802_720P50  =	    0x01,
TP2802_720P60  =    	0x00,
TP2802_SD      =        0x06,
INVALID_FORMAT =		0x07,
TP2802_720P25V2=	    0x0D,
TP2802_720P30V2=		0x0C,
TP2802_PAL	   =        0x08,
TP2802_NTSC	   =    	0x09,
TP2802_HALF1080P25  =	0x43,
TP2802_HALF1080P30  =	0x42,
TP2802_HALF720P25   =	0x45,
TP2802_HALF720P30   =   0x44,
TP2802_HALF720P50   =	0x41,
TP2802_HALF720P60   =   0x40,
//TP2802_A1080P25     =	0x13,
//TP2802_A1080P30     =   0x12,
//TP2802_AHALF1080P25 =	0x53,
//TP2802_AHALF1080P30 =	0x52,
//TP2802_A720P25      =   0x1D,
//TP2802_A720P30      =   0x1C,
TP2802_3M18         =   0x20,   //2048x1536@18.75 for TVI
TP2802_5M12         =   0x21,   //2592x1944@12.5 for TVI
TP2802_4M15         =   0x22,   //2688x1520@15 for TVI
TP2802_3M20         =   0x23,   //2048x1536@20 for TVI
TP2802_4M12         =   0x24,   //2688x1520@12.5 for TVI
TP2802_6M10         =   0x25,   //3200x1800@10 for TVI
TP2802_QHD30        =   0x26,   //2560x1440@30 for TVI/HDA/HDC
TP2802_QHD25        =   0x27,   //2560x1440@25 for TVI/HDA/HDC
TP2802_QHD15        =   0x28,   //2560x1440@15 for HDA
TP2802_QXGA18       =   0x29,   //2048x1536@18 for HDA/TVI
TP2802_QXGA30       =   0x2A,   //2048x1536@30 for HDA
TP2802_QXGA25       =   0x2B,   //2048x1536@25 for HDA
TP2802_4M30         =   0x2C,   //2688x1520@30 for TVI(for future)
TP2802_4M25         =   0x2D,   //2688x1520@25 for TVI(for future)
TP2802_5M20         =   0x2E,   //2592x1944@20 for TVI/HDA
TP2802_8M15         =   0x2f,   //3840x2160@15 for TVI
TP2802_8M12         =   0x30,   //3840x2160@12.5 for TVI
TP2802_1080P15      =   0x31,   //1920x1080@15 for TVI
TP2802_1080P60      =   0x32,   //1920x1080@60 for TVI
TP2802_960P30       =   0x33,   //1280x960@30 for TVI
TP2802_1080P20      =   0x34    //1920x1080@20 for TVI
};

enum{
    CH_1=0,   //
    CH_2=1,    //
    CH_3=2,    //
    CH_4=3,     //
    CH_ALL=4,
    DATA_PAGE=5,
    AUDIO_PAGE=9
};

enum{
    STD_TVI,
    STD_HDA,
    STD_HDC,
    STD_HDA_DEFAULT,
    STD_HDC_DEFAULT
};

#define CHANNELS_PER_CHIP 	4
#define MAX_CHIPS 	1
#define SUCCESS				0
#define FAILURE				-1

void TVI_Init(void);

#endif


