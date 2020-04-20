/* 
 * Copyright (c) 2018, Chips&Media
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
//#include "dev.h"
//#include "../../en675.h"
#include <string.h>

#include <time.h>
#include <getopt.h>
#include "cnm_app.h"
#include "encoder_listener.h"
//#include "bw_monitor.h" //ksj porting for EN675

typedef struct CommandLineArgument{
    int argc;
    char **argv;
}CommandLineArgument;


static void Help(struct OptionExt *opt, const char *programName)
{
    int i;

    VLOG(INFO, "------------------------------------------------------------------------------\n");
    VLOG(INFO, "%s(API v%d.%d.%d)\n", GetBasename(programName), API_VERSION_MAJOR, API_VERSION_MINOR, API_VERSION_PATCH);
    VLOG(INFO, "\tAll rights reserved by Chips&Media(C)\n");
    VLOG(INFO, "------------------------------------------------------------------------------\n");
    VLOG(INFO, "%s [option] --input bistream\n", GetBasename(programName));
    VLOG(INFO, "-h                          help\n");
    VLOG(INFO, "-n [num]                    output frame number, -1,0:unlimited encoding(depends on YUV size)\n");
    VLOG(INFO, "-v                          print version information\n");
    VLOG(INFO, "-c                          compare with golden bitstream\n");

    for (i = 0;i < MAX_GETOPT_OPTIONS;i++) {
        if (opt[i].name == NULL)
            break;
        VLOG(INFO, "%s", opt[i].help);
    }
}

static BOOL CheckTestConfig(TestEncConfig *testConfig)
{
    if ( (testConfig->compareType & (1<<MODE_SAVE_ENCODED)) && testConfig->bitstreamFileName[0] == 0) {
        testConfig->compareType &= ~(1<<MODE_SAVE_ENCODED);
        VLOG(ERR,"You want to Save bitstream data. Set the path\n");
        return FALSE;
    }

    if ( (testConfig->compareType & (1<<MODE_COMP_ENCODED)) && testConfig->ref_stream_path[0] == 0) {
        testConfig->compareType &= ~(1<<MODE_COMP_ENCODED);
        VLOG(ERR,"You want to Compare bitstream data. Set the path\n");
        return FALSE;
    }

    return TRUE;
}
#ifdef EYENIX_TEST
static BOOL ParseArgumentAndSetTestConfig(CommandLineArgument argument, TestEncConfig* testConfig) {
/*
	default=0						 #Default Option test:
                                 	 #default=0   off
                                 	 #default=1   on
                                 	 #-99 all random

	secondary-axi=0 				 #0 ~ 3,(bitmask - 0x01:RDO, 0x02:LF),			 -99(random)

	yuv_src_mode=0					 #0:planar, 1:nv12, 2:nv21, 3:YUYV(packed), 4:YVYU, 5:UYVY, 6:VYUY					   -99(random)
	srcFormat3p4b=0 				 #three pixel four bytes						 -99(random)
	stream-endian=31				 #16 ~ 31,										 -99(random)
	frame-endian=31 				 #16 ~ 31,										 -99(random)
	source-endian=31				 #16 ~ 31,										 -99(random)
	rotAngle=0						 #0, 90, 180, 270,								 -99(random)
	mirDir=0						 #0 ~ 3,										 -99(random)
	bsmode=1						 #0:ringbuffer, 1:linebuffer					 -99(random)

	cframe=0						 #for Vimicro fbc50s							 -99(random)
	cframelossless=1				 #for Vimicro fbc50s							 -99(random)
	cframetx16y=0					 #for Vimicro fbc50s							 -99(random)
	cframetx16c=0					 #for Vimicro fbc50s							 -99(random)

	lowLatencyMode=0				 #0~3											 -99(random)

	MODE_COMP_ENCODED=1 			 #-c  compare between FPGA output & ref-c output

*/

    //if (!strcmp(options[idx].name, "output")) {
    //    osal_memcpy(testConfig->bitstreamFileName, optarg, strlen(optarg));
    //    ChangePathStyle(testConfig->bitstreamFileName);
    //} else if (!strcmp(options[idx].name, "input")) {
    //    strcpy(testConfig->optYuvPath, optarg);
    //    ChangePathStyle(testConfig->optYuvPath);
    //} else if (!strcmp(options[idx].name, "codec")) {

#ifdef	EYENIX_AVC_TEST
		testConfig->stdMode = STD_AVC;//STD_HEVC;//(CodStd)atoi(optarg);
#else
        testConfig->stdMode = STD_HEVC;//STD_HEVC;//(CodStd)atoi(optarg);
#endif
    //} else if (!strcmp(options[idx].name, "cfgFileName")) {
    //    osal_memcpy(testConfig->cfgFileName, optarg, strlen(optarg));
    //} else if (!strcmp(options[idx].name, "coreIdx")) {
        testConfig->coreIdx = 0;//0;//atoi(optarg);
    //} else if (!strcmp(options[idx].name, "picWidth")) {
        testConfig->picWidth = 416;//3840;//3840;//1920;//416;//atoi(optarg); //2048x1080 ok, 3840x1080 x, 2160x1080 0
    //} else if (!strcmp(options[idx].name, "picHeight")) {
        testConfig->picHeight = 240;//2160;//1080;//240;//256;//240;//atoi(optarg);
    //} else if (!strcmp(options[idx].name, "kbps")) {
        testConfig->kbps = 1000;//atoi(optarg);
    //} else if (!strcmp(options[idx].name, "ringBuffer")) {
    //    Uint32 temp;
    //    temp = atoi(optarg);
    //    if ( temp == 1 ) {
//            testConfig->ringBufferEnable = 1;
//            testConfig->ringBufferWrapEnable = 1;
    //    }
    //    if ( temp == 2 ) {
    //        testConfig->ringBufferEnable = 1;
    //        testConfig->ringBufferWrapEnable = 0;
    //    }
    //} else if (!strcmp(options[idx].name, "srcReleaseInt")) {
    //    testConfig->srcReleaseIntEnable = atoi(optarg);
    //} else if (!strcmp(options[idx].name, "lineBufInt")) {
    //    testConfig->lineBufIntEn = atoi(optarg);
    //} else if (!strcmp(options[idx].name, "subFrameSyncEn")) {

        testConfig->subFrameSyncEn = 0;//atoi(optarg);
    //} else if (!strcmp(options[idx].name, "lowLatencyMode")) {
        testConfig->lowLatencyMode = 0;//atoi(optarg);
    //} else if (!strcmp(options[idx].name, "loop-count")) {
    //    testConfig->loopCount = atoi(optarg);
    //} else if (!strcmp(options[idx].name, "enable-cbcrInterleave")) {
    //    testConfig->cbcrInterleave = 1;
    //} else if (!strcmp(options[idx].name, "nv21")) {
    //    testConfig->nv21 = atoi(optarg);
    //} else if (!strcmp(options[idx].name, "packedFormat")) {
    //    testConfig->packedFormat = atoi(optarg);
    //} else if (!strcmp(options[idx].name, "rotAngle")) {
        testConfig->rotAngle = 0;//atoi(optarg);
    //} else if (!strcmp(options[idx].name, "mirDir")) {
        testConfig->mirDir = 0;//atoi(optarg);
    //} else if (!strcmp(options[idx].name, "secondary-axi")) {
        testConfig->secondaryAXI = 0;//atoi(optarg);
    //} else if (!strcmp(options[idx].name, "frame-endian")) {

        testConfig->frame_endian = VDI_128BIT_LITTLE_ENDIAN;//31;//atoi(optarg);
    //} else if (!strcmp(options[idx].name, "stream-endian")) {
        testConfig->stream_endian = VDI_128BIT_LITTLE_ENDIAN;//31;//(EndianMode)atoi(optarg);
    //} else if (!strcmp(options[idx].name, "source-endian")) {
        testConfig->source_endian = VDI_128BIT_LITTLE_ENDIAN;//31;//atoi(optarg);
    //} else if (!strcmp(options[idx].name, "ref_stream_path")) {
    //    osal_memcpy(testConfig->ref_stream_path, optarg, strlen(optarg));
    //    ChangePathStyle(testConfig->ref_stream_path);
    //} else if (!strcmp(options[idx].name, "srcFormat")) {
    //    Uint32 temp;
    //    temp = atoi(optarg);
        testConfig->srcFormat = FORMAT_420;
    //    if (temp == 1) {
    //        testConfig->srcFormat = FORMAT_420_P10_16BIT_MSB;
    //    }
    //    else if (temp == 2) {
    //        testConfig->srcFormat = FORMAT_420_P10_16BIT_LSB;
    //    }
    //    else if (temp == 3) {
    //        testConfig->srcFormat = FORMAT_420_P10_32BIT_MSB;
    //    }
    //    else if (temp == 4) {
    //        testConfig->srcFormat = FORMAT_420_P10_32BIT_LSB;
    //    }
    //}

	//printf("TestConfig lowLatencyMode=0 rotAngle=0 mirDir=0 secondary-axi=0 stream-endian=31 frame-endian=31 source-endian=31 srcFormat3p4b=0 FORMAT_420 \n");

    return TRUE;
}
#else
static BOOL ParseArgumentAndSetTestConfig(CommandLineArgument argument, TestEncConfig* testConfig) {
    Int32  opt  = 0, i = 0, idx = 0;
    int    argc = argument.argc;
    char** argv = argument.argv;
    char* optString = "rbhvn:";
    struct option options[MAX_GETOPT_OPTIONS];
    struct OptionExt options_help[MAX_GETOPT_OPTIONS] = {
        {"output",                1, NULL, 0, "--output                    An output bitstream file path\n"},
        {"input",                 1, NULL, 0, "--input                     YUV file path. The value of InputFile in a cfg is replaced to this value.\n"},
        {"codec",                 1, NULL, 0, "--codec                     codec index, 12:HEVC, 0:AVC\n"},
        {"cfgFileName",           1, NULL, 0, "--cfgFileName               cfg file path\n"},
        {"coreIdx",               1, NULL, 0, "--coreIdx                   core index: default 0\n"},
        {"picWidth",              1, NULL, 0, "--picWidth                  source width\n"},
        {"picHeight",             1, NULL, 0, "--picHeight                 source height\n"},
        {"kbps",                  1, NULL, 0, "--kbps                      RC bitrate in kbps. In case of without cfg file, if this option has value then RC will be enabled\n"},
#ifdef SUPPORT_SOURCE_RELEASE_INTERRUPT
        {"srcReleaseInt",         1, NULL, 0, "--srcReleaseInt             1 : enable source release Interrupt\n"},
#endif
        {"lineBufInt",            1, NULL, 0, "--lineBufInt                1 : enable linebuffer interrupt\n"},
        {"subFrameSyncEn",        1, NULL, 0, "--subFrameSyncEn            subFrameSync 0: off 1: on, default off\n"},
        {"lowLatencyMode",        1, NULL, 0, "--lowLatencyMode            bit[1]: low latency interrupt enable, bit[0]: fast bitstream-packing enable\n"},
        {"loop-count",            1, NULL, 0, "--loop-count                integer number. loop test, default 0\n"},
        {"enable-cbcrInterleave", 0, NULL, 0, "--enable-cbcrInterleave     enable cbcr interleave\n"},
        {"nv21",                  1, NULL, 0, "--nv21                      enable NV21(must set enable-cbcrInterleave)\n"},
        {"packedFormat",          1, NULL, 0, "--packedFormat              1:YUYV, 2:YVYU, 3:UYVY, 4:VYUY\n"},
        {"rotAngle",              1, NULL, 0, "--rotAngle                  rotation angle(0,90,180,270), Not supported on WAVE420L, WAVE525, WAVE521C_DUAL\n"},
        {"mirDir",                1, NULL, 0, "--mirDir                    1:Vertical, 2: Horizontal, 3:Vert-Horz, Not supported on WAVE420L, WAVE525, WAVE521C_DUAL\n"},
        {"secondary-axi",         1, NULL, 0, "--secondary-axi             0~3: bit mask values, Please refer programmer's guide or datasheet\n"
                                              "                            1: RDO, 2: LF\n"},
        {"frame-endian",          1, NULL, 0, "--frame-endian              16~31, default 31(LE) Please refer programmer's guide or datasheet\n"},
        {"stream-endian",         1, NULL, 0, "--stream-endian             16~31, default 31(LE) Please refer programmer's guide or datasheet\n"},
        {"source-endian",         1, NULL, 0, "--source-endian             16~31, default 31(LE) Please refer programmer's guide or datasheet\n"},
        {"ref_stream_path",       1, NULL, 0, "--ref_stream_path           golden data which is compared with encoded stream when -c option\n"},
        {"srcFormat",             1, NULL, 0, "--srcFormat                 0:8bit, 1:10bit 1P2B_MSB, 2:10bit 1P2B_LSB, 3:10bit 3P4B_MSB, 4:10bit 3P4B_LSB\n"
                                              "                            1P2B : 10bit 1pixel 2bytes source format\n"
                                              "                            3P4B : 10bit 3pixel 4bytes srouce format\n"},
        {NULL,                    0, NULL, 0},
    };
  

    for (i = 0; i < MAX_GETOPT_OPTIONS;i++) {
        if (options_help[i].name == NULL)
            break;
        osal_memcpy(&options[i], &options_help[i], sizeof(struct option));
    }

    while ((opt=getopt_long(argc, argv, optString, options, (int *)&idx)) != -1) {
        switch (opt) {
        case 'n':
            testConfig->outNum = atoi(optarg);
            break;
        case 'c':
            testConfig->compareType |= (1 << MODE_COMP_ENCODED);
            VLOG(TRACE, "Stream compare Enable\n");
            break;
        case 'h':
            Help(options_help, argv[0]);
            exit(0);
        case 0:
            if (!strcmp(options[idx].name, "output")) {
                osal_memcpy(testConfig->bitstreamFileName, optarg, strlen(optarg));
                ChangePathStyle(testConfig->bitstreamFileName);
            } else if (!strcmp(options[idx].name, "input")) {
                strcpy(testConfig->optYuvPath, optarg);
                ChangePathStyle(testConfig->optYuvPath);
            } else if (!strcmp(options[idx].name, "codec")) {
                testConfig->stdMode = (CodStd)atoi(optarg);
            } else if (!strcmp(options[idx].name, "cfgFileName")) {
                osal_memcpy(testConfig->cfgFileName, optarg, strlen(optarg));
            } else if (!strcmp(options[idx].name, "coreIdx")) {
                testConfig->coreIdx = atoi(optarg);
            } else if (!strcmp(options[idx].name, "picWidth")) {
                testConfig->picWidth = atoi(optarg);
            } else if (!strcmp(options[idx].name, "picHeight")) {
                testConfig->picHeight = atoi(optarg);
            } else if (!strcmp(options[idx].name, "kbps")) {
                testConfig->kbps = atoi(optarg);
#ifdef SUPPORT_SOURCE_RELEASE_INTERRUPT
            } else if (!strcmp(options[idx].name, "srcReleaseInt")) {
                testConfig->srcReleaseIntEnable = atoi(optarg);
#endif
            } else if (!strcmp(options[idx].name, "lineBufInt")) {
                testConfig->lineBufIntEn = atoi(optarg);
            } else if (!strcmp(options[idx].name, "subFrameSyncEn")) {
                testConfig->subFrameSyncEn = atoi(optarg);
            } else if (!strcmp(options[idx].name, "lowLatencyMode")) {
                testConfig->lowLatencyMode = atoi(optarg);
            } else if (!strcmp(options[idx].name, "loop-count")) {
                testConfig->loopCount = atoi(optarg);
            } else if (!strcmp(options[idx].name, "enable-cbcrInterleave")) {
                testConfig->cbcrInterleave = 1;
            } else if (!strcmp(options[idx].name, "nv21")) {
                testConfig->nv21 = atoi(optarg);
            } else if (!strcmp(options[idx].name, "packedFormat")) {
                testConfig->packedFormat = atoi(optarg);
            } else if (!strcmp(options[idx].name, "rotAngle")) {
                testConfig->rotAngle = atoi(optarg);
            } else if (!strcmp(options[idx].name, "mirDir")) {
                testConfig->mirDir = atoi(optarg);
            } else if (!strcmp(options[idx].name, "secondary-axi")) {
                testConfig->secondaryAXI = atoi(optarg);
            } else if (!strcmp(options[idx].name, "frame-endian")) {
                testConfig->frame_endian = atoi(optarg);
            } else if (!strcmp(options[idx].name, "stream-endian")) {
                testConfig->stream_endian = (EndianMode)atoi(optarg);
            } else if (!strcmp(options[idx].name, "source-endian")) {
                testConfig->source_endian = atoi(optarg);
            } else if (!strcmp(options[idx].name, "ref_stream_path")) {
                osal_memcpy(testConfig->ref_stream_path, optarg, strlen(optarg));
                ChangePathStyle(testConfig->ref_stream_path);
            } else if (!strcmp(options[idx].name, "srcFormat")) {
                Uint32 temp;
                temp = atoi(optarg);
                testConfig->srcFormat = FORMAT_420;
                if (temp == 1) {
                    testConfig->srcFormat = FORMAT_420_P10_16BIT_MSB;
                }
                else if (temp == 2) {
                    testConfig->srcFormat = FORMAT_420_P10_16BIT_LSB;
                }
                else if (temp == 3) {
                    testConfig->srcFormat = FORMAT_420_P10_32BIT_MSB;
                }
                else if (temp == 4) {
                    testConfig->srcFormat = FORMAT_420_P10_32BIT_LSB;
                }
            } else {
                VLOG(ERR, "not exist param = %s\n", options[idx].name);
                Help(options_help, argv[0]);
                return FALSE;
            }
            break;
        default:
            VLOG(ERR, "%s\n", optarg);
            Help(options_help, argv[0]);
            return FALSE;
        }
    }
    VLOG(INFO, "\n");

    return TRUE;
}
#endif

//ENCODIER ////////////////////////////////////////////////////////////////////////////////////////////////////]]
int main_enc_test(int argc, char **argv)
{
#if 0
    char*               fwPath     = NULL;
	Uint32	retval;
    RetCode         retCode;
	CNMComponentConfig  config;
	Uint32 firmwareSize;
	Uint32 pc;
	PhysicalAddress codeBase, tempBase;
    Uint32          codeSize, tempSize;
    int         changes;
    int         sys_endian;
	int endian;
    BOOL        byteChange, wordChange, dwordChange, lwordChange;
	BYTE *pchadd;
	Uint32 coreIdx;
    Uint32          i, regVal, remapSize;
    Uint16*             pusBitCode;
    Uint32              sizeInWord;


	
	CodecInst               pTestCodecInst;
	CodecInst*              instance = &pTestCodecInst;
	EncInfo    pTestEncInfo;
	EncInfo*    pEncInfo = &pTestEncInfo;
	int productId;
    TestEncConfig       testConfig;


	
	instance->CodecInfo = pEncInfo;

	
	instance->inUse;
    instance->instIndex;
    //instance->coreIdx;
    instance->codecMode;
    instance->codecModeAux;    
    instance->productId;
    instance->loggingEnable;
    instance->isDecoder;
	EncOpenParam *param = &instance->CodecInfo->encInfo.openParam; //&testparam;
	EncOpenParam *pop = param;//&testparam;

	instance->coreIdx = coreIdx = 0;


#if 0
	BYTE *testmem;
	UINT testaddr;
	printf("malloc test1\n");
	//printf("testmem addr [%x] \n", testmem);
	testaddr = 0xC0000000;//0xA0000000;//0x80000000;
	//testmem = (void *)malloc(8);
	//testmem = (BYTE *)(0x80000000);//testmem = osal_malloc(1);
	testmem = (BYTE*)testaddr;
	printf("malloc test1 testmem addr %x data %x \n", testmem, *testmem);

	testaddr = 0xA0000000;//0x80000000;
	testmem = (BYTE*)testaddr;
	printf("malloc test2 testmem addr %x data %x \n", testmem, *testmem);

	testaddr = 0x90000000;//0x80000000;
	testmem = (BYTE*)testaddr;
	printf("malloc test3 testmem addr %x data %x \n", testmem, *testmem);

	testaddr = 0x80000000;//0x80000000;
	testmem = (BYTE*)testaddr;
	printf("malloc test4 testmem addr %x data %x \n", testmem, *testmem);
#endif

#endif

    char*               fwPath     = NULL;
    TestEncConfig       testConfig;
    CommandLineArgument argument;
    CNMComponentConfig  config;
    CNMTask             task;
    Component           yuvFeeder;
    Component           encoder;
    Component           reader;
    Uint32              sizeInWord;
    Uint16*             pusBitCode;
    BOOL                ret;
    BOOL                testResult;
    EncListenerContext  lsnCtx;

    osal_memset(&argument, 0x00, sizeof(CommandLineArgument));
    osal_memset(&config,   0x00, sizeof(CNMComponentConfig));

    InitLog();
    SetDefaultEncTestConfig(&testConfig);
    argument.argc = argc;
    argument.argv = argv;
    if (ParseArgumentAndSetTestConfig(argument, &testConfig) == FALSE) {
        VLOG(ERR, "fail to ParseArgumentAndSetTestConfig()\n");
        return 1;
    }

    /* Need to Add */
    /* FW load & show version case*/
    testConfig.productId = (ProductId)VPU_GetProductId(testConfig.coreIdx); ////////////[[
    	//VPU_GetProductId
    	//WaveVpuGetProductId
		//get product number
		//printf("main_enc_test!\n");
		//Uint32 retval = VpuReadReg(0, W5_PRODUCT_NUMBER);	
		//printf("Check productId(%d)\n", retval);
	    //////////////////////////////////////////////////////////////////////////////////]]
	    

    //printf("Test!! memset all\n");
    //memset(0x80000000, 0x00, 64*1024*1024);
    //printf("Test!! done!\n");


	if (CheckTestConfig(&testConfig) == FALSE) {
        VLOG(ERR, "fail to CheckTestConfig()\n");
        return 1;
    }

    switch (testConfig.productId) {
    case PRODUCT_ID_521:    fwPath = CORE_6_BIT_CODE_FILE_PATH; break;
    default:
        VLOG(ERR, "Unknown product id: %d\n", testConfig.productId);
        return 1;
    }

    VLOG(INFO, "FW PATH = %s\n", fwPath);

#if 0
    productId = s_ProductIds[coreIdx];	
	instance->productId = productId;
	if(PRODUCT_ID_521 != instance->productId)
	{
		printf("product id failed %d \n",instance->productId);
		return FALSE;
	}
#endif

	//printf("FW PATH = 0x%x \n", FILE_BUFFER_BASE);

	//bin
	//load_image G:/EN675/Eclipse_setup/chagall.bin 0x80500000 bin
	//verify_image G:/EN675/Eclipse_setup/chagall.bin 0x80500000 bin
	//mdb 0x80500000 16
	//image
	//load_image G:/EN675/Eclipse_setup/BasketballPass_416x240_5.yuv 0x80600000 bin
	//verify_image G:/EN675/Eclipse_setup/BasketballPass_416x240_5.yuv 0x80600000 bin
	//mdb 0x80600000 16

     if (LoadFirmware(testConfig.productId, (Uint8**)&pusBitCode, &sizeInWord, fwPath) < 0) {
        VLOG(ERR, "%s:%d Failed to load firmware: %s\n", __FUNCTION__, __LINE__, fwPath);
        return 1;
    }
    //FILE_BUFFER_BASE;//(Uint8*)pusBitCode
    //FILE_BUFFER_SIZE;//sizeInWord

	//char* filebase = FILE_BUFFER_BASE;
	//config.testEncConfig = testConfig;
    //config.bitcode       = filebase;//(Uint8*)pusBitCode;
    //config.sizeOfBitcode = FILE_BUFFER_SIZE;//sizeInWord;
    //config.testEncConfig.subFrameSyncMode = REGISTER_BASE_SUB_FRAME_SYNC;//0;//



    config.testEncConfig = testConfig;
    config.bitcode       = (Uint8*)pusBitCode;
    config.sizeOfBitcode = sizeInWord;
    config.testEncConfig.subFrameSyncMode = WIRED_BASE_SUB_FRAME_SYNC;//REGISTER_BASE_SUB_FRAME_SYNC;

    if (SetupEncoderOpenParam(&config.encOpenParam, &config.testEncConfig, NULL) == FALSE)
    {
    	//printf("SetupEncoderOpenParam fail\n", FILE_BUFFER_BASE);
        return 1;
    }


    CNMAppInit();

    task       = CNMTaskCreate();
    yuvFeeder  = ComponentCreate("yuvfeeder", &config);
    encoder    = ComponentCreate("wave_encoder",   &config);
    reader     = ComponentCreate("reader",         &config);

    CNMTaskAdd(task, yuvFeeder);
    CNMTaskAdd(task, encoder);
    CNMTaskAdd(task, reader);

    CNMAppAdd(task);

    if ((ret=SetupEncListenerContext(&lsnCtx, &config)) == TRUE) {
        ComponentRegisterListener(encoder, COMPONENT_EVENT_ENC_ALL, EncoderListener, (void*)&lsnCtx);
        ret = CNMAppRun();
    }
    else {
        CNMAppStop();
    }

    osal_free(pusBitCode);
    ClearEncListenerContext(&lsnCtx);

    testResult = (ret == TRUE && lsnCtx.match == TRUE && lsnCtx.matchOtherInfo == TRUE);
    if (testResult == TRUE) VLOG(INFO, "[RESULT] SUCCESS\n");
    else                    VLOG(ERR,  "[RESULT] FAILURE\n");

    if ( CNMErrorGet() != 0 )
        return CNMErrorGet();

    return (testResult == TRUE) ? 0 : 1;

#if 0
	//YuvFeeder
	//CreateYuvFeeder(&conYuv, &config);
	//yuvYuvFeeder_Create(&implyuv, NULL, 

#ifdef TESTON6
	//printf("CE\n");

	CreateEncoder(&wave5com, &config);	
	//printf("EC0\n");
	BOOL done;
	PrepareEncoder(&wave5com, &done);

	((EncoderContext*)wave5com.context)->state = ENCODER_STATE_OPEN;
	ExecuteEncoder(&wave5com, NULL, NULL);
	//printf("EC1\n");       
	((EncoderContext*)wave5com.context)->state = ENCODER_STATE_INIT_SEQ;
	ExecuteEncoder(&wave5com, NULL, NULL);
	//printf("EC2\n");
	((EncoderContext*)wave5com.context)->state = ENCODER_STATE_REGISTER_FB;
	ExecuteEncoder(&wave5com, NULL, NULL);
    //printf("EC3\n");    
    ((EncoderContext*)wave5com.context)->state = ENCODER_STATE_ENCODE_HEADER;
	ExecuteEncoder(&wave5com, NULL, NULL);
#if 1	
	{
		ENC_INT_STATUS			intStatus;		
		EncoderContext* ctx 			= (EncoderContext*)wave5com.context;
		
		if ((intStatus=HandlingInterruptFlag(&wave5com)) == ENC_INT_STATUS_TIMEOUT) {
			printf("ENC_INT_STATUS_TIMEOUT\n");
		}
		else if (intStatus == ENC_INT_STATUS_FULL || intStatus == ENC_INT_STATUS_LOW_LATENCY) {
			printf("ENC_INT_STATUS_FULL\n");
		}
		else if (intStatus == ENC_INT_STATUS_SRC_RELEASED) {
			printf("ENC_INT_STATUS_SRC_RELEASED\n");
		}
		else if (intStatus == ENC_INT_STATUS_NONE) {
			printf("ENC_INT_STATUS_NONE\n");
		}
		else{
			
			EncOutputInfo			encOutputInfo;
			ENC_QUERY_WRPTR_SEL 	encWrPtrSel 	= GET_ENC_PIC_DONE_WRPTR;

			VPU_EncGiveCommand(ctx->handle, ENC_WRPTR_SEL, &encWrPtrSel);
			//osal_memset(&encOutputInfo, 0x00, sizeof(EncOutputInfo));
			
			encOutputInfo.result = VPU_EncGetOutputInfo(ctx->handle, &encOutputInfo);
			
			if (encOutputInfo.result == RETCODE_REPORT_NOT_READY) {
				printf("RETCODE_REPORT_NOT_READY\n");
			}
			else if (encOutputInfo.result == RETCODE_VLC_BUF_FULL) {
				printf("RETCODE_VLC_BUF_FULL\n");
			}
			else if (encOutputInfo.result != RETCODE_SUCCESS) {
				printf("FAILED IRQ\n");
			}
			else {
				;/* SUCCESS */
			}

		}
	}
#endif

	((EncoderContext*)wave5com.context)->state = ENCODER_STATE_ENCODING;
	ExecuteEncoder(&wave5com, (PortContainer*)&yuvin, (PortContainer*)&ESout);
	
	
	((EncoderContext*)wave5com.context)->state = ENCODER_STATE_ENCODING;
	ExecuteEncoder(&wave5com, (PortContainer*)&yuvin, (PortContainer*)&ESout);

	
#endif
	//while(1);
	return 0;
#endif

}

