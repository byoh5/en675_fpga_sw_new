#include "dev.h"

#include "enx_freertos.h"
#include "enx_lwip.h"
#include "lwip/apps/httpd.h"

#include "networkif.h"

#include "cgissi.h"

#if (LOAD_FS_SDCARD==1)
#include "sdcard.h"
#include "muxer_videnc.h"
#endif

#include "ipchange.h"
#include "videochange.h"

/* ------------------------------------------------------------------------ */
#if LWIP_HTTPD_SSI

char sensor_name[32] = "EN675_TEST";

static void SSI_entry_state(char *pcInsert, int iInsertLen)
{
	_Rprintf("%s\n", __func__);
#if 0
	char strXmlData[2048] = {0};

	snprintf(strXmlData, iInsertLen, "%s<SYSTEM>%s", CRLF, CRLF);

	// DEVINFO
	snprintf(strXmlData, iInsertLen, "%s<DEVINFO>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<VERSION>%u</VERSION>%s", strXmlData, gtSystem.u32Version, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</DEVINFO>%s", strXmlData, CRLF);
#if 0
	// USER
	snprintf(strXmlData, iInsertLen, "%s<USER>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<ID>%s</ID>%s", strXmlData, gtNetwork.strUserId, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<PASSWORD>%s</PASSWORD>%s", strXmlData, "[NULL]", CRLF);
#if (ENX_FTPD_use==1)
	snprintf(strXmlData, iInsertLen, "%s<FTPSERVER>%u</FTPSERVER>%s", strXmlData, gtNetwork.bFtpServer, CRLF);
#else
	snprintf(strXmlData, iInsertLen, "%s<FTPSERVER>"XML_NA"</FTPSERVER>"CRLF, strXmlData);
#endif

#if (LOAD_FS_SDCARD==1)
	snprintf(strXmlData, iInsertLen, "%s<SDAVISAVE>%u</SDAVISAVE>%s", strXmlData, gtUser.bSdVidSave, CRLF);
#else
	snprintf(strXmlData, iInsertLen, "%s<SDAVISAVE>"XML_NA"</SDAVISAVE>%s", strXmlData, CRLF);
#endif

#ifdef __AUDIO__
	snprintf(strXmlData, iInsertLen, "%s<AUDIO>%u</AUDIO>%s", strXmlData, gtUser.bAudio, CRLF);
#else
	snprintf(strXmlData, iInsertLen, "%s<AUDIO>"XML_NA"</AUDIO>%s", strXmlData, CRLF);
#endif

	snprintf(strXmlData, iInsertLen, "%s</USER>%s", strXmlData, CRLF);

	// DATE
	snprintf(strXmlData, iInsertLen, "%s<DATE>%s", strXmlData, CRLF);
#if (ENX_SNTPC_use==1)
	snprintf(strXmlData, iInsertLen, "%s<SNTPSVR>%s</SNTPSVR>%s", strXmlData, gtNetwork.strSntpSvr, CRLF);
#endif
	snprintf(strXmlData, iInsertLen, "%s<SYSTIME>%u</SYSTIME>%s", strXmlData, gptMsgShare.TIME, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<UPTIME>%lu</UPTIME>%s", strXmlData, gptMsgShare.UPTIME, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<TIMEZONE>%d</TIMEZONE>%s", strXmlData, gtUser.nTimeZone&0xFF, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<TIMEZONEM>%d</TIMEZONEM>%s", strXmlData, (gtUser.nTimeZone>>8)&&0xFF, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</DATE>%s", strXmlData, CRLF);
#endif
	snprintf(strXmlData, iInsertLen, "%s</SYSTEM>%s", strXmlData, CRLF);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
#ifdef __WIFI__
#if 0
	snprintf(strXmlData, iInsertLen, "%s<WIFI>%s", strXmlData, CRLF);

	// WiFi-AP gtNetwork.UAPcfg
	snprintf(strXmlData, iInsertLen, "%s<AP_MODE>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<IP>%IP</IP>%s", strXmlData, gtNetwork.UAPcfg.iptLocalip.addr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<NETMASK>%IP</NETMASK>%s", strXmlData, gtNetwork.UAPcfg.iptNetmask.addr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DHCPUSE>%u</DHCPUSE>%s", strXmlData, gtNetwork.UAPcfg.un8DHCPServer, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</AP_MODE>%s", strXmlData, CRLF);

	// WiFi-STA gtNetwork.STAcfg
	snprintf(strXmlData, iInsertLen, "%s<STA_MODE>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<IP>%IP</IP>%s", strXmlData, gtNetwork.STAcfg.iptLocalip.addr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<NETMASK>%IP</NETMASK>%s", strXmlData, gtNetwork.STAcfg.iptNetmask.addr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<GATEWAY>%IP</GATEWAY>%s", strXmlData, gtNetwork.STAcfg.iptGateway.addr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DNS0>%IP</DNS0>%s", strXmlData, gtNetwork.STAcfg.iptDNS0.addr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DNS1>%IP</DNS1>%s", strXmlData, gtNetwork.STAcfg.iptDNS1.addr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DHCPUSE>%u</DHCPUSE>%s", strXmlData, gtNetwork.STAcfg.un8DHCPSwitch, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</STA_MODE>%s", strXmlData, CRLF);

	snprintf(strXmlData, iInsertLen, "%s</WIFI>%s", strXmlData, CRLF);
#endif
#endif

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	snprintf(strXmlData, iInsertLen, "%s<NETWORK>%s", strXmlData, CRLF);

	// TCP
	// gtUser.nIpAddr, gtUser.nNetMask, gtUser.nGateway, gtUser.nDnsSvr0, gtUser.nDnsSvr1, gtUser.bUseDhcp
	snprintf(strXmlData, iInsertLen, "%s<TCP>%s", strXmlData, CRLF);
#if defined(__ETH__)
	snprintf(strXmlData, iInsertLen, "%s<IP>%IP</IP>%s", strXmlData, gtNetwork.nIpAddr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<NETMASK>%IP</NETMASK>%s", strXmlData, gtNetwork.nNetMask, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<GATEWAY>%IP</GATEWAY>%s", strXmlData, gtNetwork.nGateway, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DNS0>%IP</DNS0>%s", strXmlData, gtNetwork.nDnsSvr0, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DNS1>%IP</DNS1>%s", strXmlData, gtNetwork.nDnsSvr1, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DHCPUSE>%u</DHCPUSE>%s", strXmlData, gtNetwork.bUseDhcp, CRLF);
#endif
	snprintf(strXmlData, iInsertLen, "%s</TCP>%s", strXmlData, CRLF);

	// UPNP
	// gtUser.strDeviceId, gtUser.strDeviceName
	snprintf(strXmlData, iInsertLen, "%s<UPNP>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DEVICEID>%s</DEVICEID>%s", strXmlData, gtUser.strDeviceId, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DEVICENAME>%s</DEVICENAME>%s", strXmlData, gtUser.strDeviceName, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</UPNP>%s", strXmlData, CRLF);

	snprintf(strXmlData, iInsertLen, "%s</NETWORK>%s", strXmlData, CRLF);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	snprintf(strXmlData, iInsertLen, "%s<VIDEO>%s", strXmlData, CRLF);

	snprintf(strXmlData, iInsertLen, "%s<SENSOR>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<NAME>%s</NAME>%s", strXmlData, sensor_name, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<SIZE_W>%u</SIZE_W>%s", strXmlData, SENSOR_SIZE_W, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<SIZE_H>%u</SIZE_H>%s", strXmlData, SENSOR_SIZE_H, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<MAXFPS>%d</MAXFPS>%s", strXmlData, gptMsgShare.VIDEO_FPS, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</SENSOR>%s", strXmlData, CRLF);

	snprintf(strXmlData, iInsertLen, "%s<RTSPPORT>%u</RTSPPORT>%s", strXmlData, gtNetwork.nRtspPort, CRLF);

	// VIDEO-H264_1
	// gtUser.uviH264.eResolution, gtUser.uviH264.byQuality, gtUser.uviH264.woIDRFrame, gtUser.uviH264.sfFPS
#ifdef __H264__
	snprintf(strXmlData, iInsertLen, "%s<H264_1>%s", strXmlData, CRLF);
#if (ENX_RTSP_use==1)
#if defined(__ETH__)
	snprintf(strXmlData, iInsertLen, "%s<URL>rtsp://%IP:%u/%s</URL>%s", strXmlData, gtNetwork.nIpAddr, gtNetwork.nRtspPort, H264URL, CRLF);
#endif
#if defined(__WIFI__)
	snprintf(strXmlData, iInsertLen, "%s<URL1>rtsp://%IP:%u/%s</URL1>%s", strXmlData, gtNetwork.UAPcfg.un32Localip, gtNetwork.nRtspPort, H264URL, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<URL2>rtsp://%IP:%u/%s</URL2>%s", strXmlData, gtNetwork.STAcfg.un32Localip, gtNetwork.nRtspPort, H264URL, CRLF);
#endif
#endif
	snprintf(strXmlData, iInsertLen, "%s<RESOLUTION>%u</RESOLUTION>%s", strXmlData, gtUser.uviH264[0].eResolution, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<QUALITY>%u</QUALITY>%s", strXmlData, gtUser.uviH264[0].nQuality, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<IDRFRAME>%u</IDRFRAME>%s", strXmlData, gtUser.uviH264[0].nIDRFrame, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<FPS>%u</FPS>%s", strXmlData, gtUser.uviH264[0].sfFPS, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<BRMODE>%u</BRMODE>%s", strXmlData, gtUser.uviH264[0].eBRMode, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<BRATE>%u</BRATE>%s", strXmlData, gtUser.uviH264[0].nBitRate, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</H264_1>%s", strXmlData, CRLF);

	// VIDEO-H264_2
	// gtUser.uviH264.eResolution, gtUser.uviH264.byQuality, gtUser.uviH264.woIDRFrame, gtUser.uviH264.sfFPS
#ifdef __H264_2CH__
	snprintf(strXmlData, iInsertLen, "%s<H264_2>%s", strXmlData, CRLF);
#if (ENX_RTSP_use==1)
#if defined(__ETH__)
	snprintf(strXmlData, iInsertLen, "%s<URL>rtsp://%IP:%u/%s</URL>%s", strXmlData, gtNetwork.nIpAddr, gtNetwork.nRtspPort, HSUBURL, CRLF);
#endif
#if defined(__WIFI__)
	snprintf(strXmlData, iInsertLen, "%s<URL1>rtsp://%IP:%u/%s</URL1>%s", strXmlData, gtNetwork.UAPcfg.un32Localip, gtNetwork.nRtspPort, HSUBURL, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<URL2>rtsp://%IP:%u/%s</URL2>%s", strXmlData, gtNetwork.STAcfg.un32Localip, gtNetwork.nRtspPort, HSUBURL, CRLF);
#endif
#endif
	snprintf(strXmlData, iInsertLen, "%s<RESOLUTION>%u</RESOLUTION>%s", strXmlData, gtUser.uviH264[1].eResolution, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<QUALITY>%u</QUALITY>%s", strXmlData, gtUser.uviH264[1].nQuality, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<IDRFRAME>%u</IDRFRAME>%s", strXmlData, gtUser.uviH264[1].nIDRFrame, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<FPS>%u</FPS>%s", strXmlData, gtUser.uviH264[1].sfFPS, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<BRMODE>%u</BRMODE>%s", strXmlData, gtUser.uviH264[1].eBRMode, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<BRATE>%u</BRATE>%s", strXmlData, gtUser.uviH264[1].nBitRate, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</H264_2>%s", strXmlData, CRLF);
#else
	snprintf(strXmlData, iInsertLen, "%s<H264_2>%s</H264_2>%s", strXmlData, XML_DISABLE, CRLF);
#endif
#else
	snprintf(strXmlData, iInsertLen, "%s<H264_1>%s</H264_1>%s", strXmlData, XML_DISABLE, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<H264_2>%s</H264_2>%s", strXmlData, XML_DISABLE, CRLF);
#endif

	// VIDEO-JEPG
	// gtUser.uviJpeg.eResolution, gtUser.uviJpeg.byQuality, gtUser.uviJpeg.sfFPS
#ifdef __JPEG__
	snprintf(strXmlData, iInsertLen, "%s<JPEG>%s", strXmlData, CRLF);
#if (ENX_RTSP_use==1)
#if defined(__ETH__)
	snprintf(strXmlData, iInsertLen, "%s<URL>rtsp://%IP:%u/%s</URL>%s", strXmlData, gtNetwork.nIpAddr, gtNetwork.nRtspPort, JPEGURL, CRLF);
#endif
#if defined(__WIFI__)
	snprintf(strXmlData, iInsertLen, "%s<URL1>rtsp://%IP:%u/%s</URL1>%s", strXmlData, gtNetwork.UAPcfg.un32Localip, gtNetwork.nRtspPort, JPEGURL, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<URL2>rtsp://%IP:%u/%s</URL2>%s", strXmlData, gtNetwork.STAcfg.un32Localip, gtNetwork.nRtspPort, JPEGURL, CRLF);
#endif
#endif
	snprintf(strXmlData, iInsertLen, "%s<RESOLUTION>%u</RESOLUTION>%s", strXmlData, gtUser.uviJpeg.eResolution, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<QUALITY>%u</QUALITY>%s", strXmlData, gtUser.uviJpeg.nQuality, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<FPS>%u</FPS>%s", strXmlData, gtUser.uviJpeg.sfFPS, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<BRMODE>%u</BRMODE>%s", strXmlData, gtUser.uviJpeg.eBRMode, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<BRATE>%u</BRATE>%s", strXmlData, gtUser.uviJpeg.nBitRate, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</JPEG>%s", strXmlData, CRLF);
#else
	snprintf(strXmlData, iInsertLen, "%s<JPEG>%s</JPEG>%s", strXmlData, XML_DISABLE, CRLF);
#endif

	snprintf(strXmlData, iInsertLen, "%s</VIDEO>%s", strXmlData, CRLF);
#endif
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	strcpy(pcInsert, strXmlData);
//	snprintf(pcInsert, iInsertLen, strXmlData);
#endif
}

static void SSI_entry_func(char *pcInsert, int iInsertLen)
{
	printf("%s\n", __func__);
	snprintf(pcInsert, iInsertLen, "\n\tisp=%u;\n\tmod_lens=%u;\n\tmod_iris=%u;\n\tsdcd=%u;\n\thevc=%u;\n\tavc=%u;\n\tjpeg=%u;\n\teth=%u;\n\twifi=%u;\n\t",
#ifdef __ISP__
	1, model_Lens, model_Iris,
#else
	0, 0, 0,
#endif
#if (LOAD_FS_SDCARD==1)
	1,
#else
	0,
#endif
#if defined(VIDEO_CODEC_H265)
	1,
#else
	0,
#endif
#if defined(VIDEO_CODEC_H264)
	1,
#else
	0,
#endif
#if defined(VIDEO_CODEC_JPEG)
	1,
#else
	0,
#endif
#if defined(__ETHERNET__)
	1,
#else
	0,
#endif
#if defined(__WIFI__)
	1);
#else
	0);
#endif
}

static void SSI_entry_tcp(char *pcInsert, int iInsertLen)
{
#if defined(__ETHERNET__)
	NetifAddress *eth0 = (NetifAddress *)&gtNetwork.naEthernet;
	es_snprintf(pcInsert, iInsertLen, "\n\tmac0=0x%x;\n\tmac1=0x%x;\n\tmac2=0x%x;\n\tmac3=0x%x;\n\tmac4=0x%x;\n\tmac5=0x%x;\n\tip='%IP';\n\tnm='%IP';\n\tgw='%IP';\n\tdn1='%IP';\n\tdn2='%IP';\n\tdhcp=%d;\n",
			gtSystem.arr8MacAddress[0], gtSystem.arr8MacAddress[1], gtSystem.arr8MacAddress[2], gtSystem.arr8MacAddress[3], gtSystem.arr8MacAddress[4], gtSystem.arr8MacAddress[5],
			eth0->u32IpAddr, eth0->u32NetMask, eth0->u32Gateway, eth0->u32DnsSvr0, eth0->u32DnsSvr1, eth0->u1UseDhcp);
#else
	UNUSED(pcInsert);
	UNUSED(iInsertLen);
#endif
}

static void SSI_entry_upnp(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n\tuid='%s';\n\tuname='%s';\n",
			gtUser.strDeviceId, gtUser.strDeviceName);
}

static void SSI_entry_user(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n\tuid='%s';\n\tpw='%s';\n\tftpserver=%d;\n\tsdsave=%d;\n\taudiostm=%d;\n",
#if 1
			"TEMP_ID", "[NULL]",
#else
			gtNetwork.strUserId, "[NULL]",
#endif
#if defined(__FILESYSTEM__)
#if (ENX_FTPD_use==1)
			gtNetwork.bFtpServer, 
#else
			-1,
#endif
#if (LOAD_FS_SDCARD==1)
			gtUser.bSdVidSave,
#else
			-1,
#endif
#else
			-1, -1,
#endif
#ifdef __AUDIO__
#if 0
			gtUser.bAudio);
#else
			-1);
#endif
#else
			-1);
#endif
}

static void SSI_entry_date(char *pcInsert, int iInsertLen)
{
	printf("%s\n", __func__);
	snprintf(pcInsert, iInsertLen, "\n"
		"\tsntpsw=%d;\n"
#if (ENX_SNTPC_use==1)
		"\tsntpsvr='%s';\n"
		"\trepetper=%u;\n"
		"\trettime=%u;\n"
		"\tretcount=%u;\n"
#endif
		"\tfirsttime=%lu;\n"
		"\tsystime=%lu;\n"
		"\tsummertime=%d;\n"
		"\ttimezone=%d;\n",
#if (ENX_SNTPC_use==1)
			gtNetwork.bUseSntp, gtNetwork.strSntpSvr, gtNetwork.nSntpPeriodic, gtNetwork.nSntpRetrySec, gtNetwork.nSntpRetryMaxcnt,
#else
			-1, 
#endif
			gptMsgShare.UPTIME, gptMsgShare.TIME + gptMsgShare.TIMEZONE, gtUser.nTimeSummer, gtUser.nTimeZone);	// SNTP서버명, 기기작동시간, 기기의시간값(sntp동기값/jiffies), TimeZone
}

static void SSI_entry_vinfo(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n\tSensorName='%s';\n\tSensorWSize=%d;\n\tSensorHSize=%d;\n\tSensorMAXfps=%d;\n\tMAXTOTAL=%d;\n\t" \
		"H5R1Max=%d;\n\tH5R1Min=%d;\n\tH5R2Max=%d;\n\tH5R2Min=%d;\n\tH4R1Max=%d;\n\tH4R1Min=%d;\n\tH4R2Max=%d;\n\tH4R2Min=%d;\n\t" \
		"JRMax=%d;\n\tJRMin=%d;\n\tHWDS0=%d;\n\tHWDS1=%d;\n\tHWDS2=%d;\n\tHWDS3=%d;\n\t" \
		"H5QMax=%d;\n\tH5QMin=%d;\n\tH5IfMax=%d;\n\tH5IfMin=%d;\n\t" \
		"H4QMax=%d;\n\tH4QMin=%d;\n\tH4IfMax=%d;\n\tH4IfMin=%d;\n\t" \
		"JQMax=%d;\n\tJQMin=%d;\n\t",
			gvsVideo[e_vsVSource1].strName, SENSOR_SIZE_W, SENSOR_SIZE_H, gvsVideo[e_vsVSource1].nFps, e_resEndorUnknown - 1,
			H265_Resolution_1st_MAX, H265_Resolution_1st_MIN, H265_Resolution_2nd_MAX, H265_Resolution_2nd_MIN,
			H264_Resolution_1st_MAX, H264_Resolution_1st_MIN, H264_Resolution_2nd_MAX, H264_Resolution_2nd_MIN,
			JPEG_Resolution_MAX, JPEG_Resolution_MIN, HW_DS0, HW_DS1, HW_DS2, HW_DS3,
#ifdef VIDEO_CODEC_H265
			H265_Quantize_MAX, H265_Quantize_MIN, H265_Idrframe_MAX, H265_Idrframe_MIN,
#else
			-1, -1, -1, -1,
#endif
#ifdef VIDEO_CODEC_H264
			H264_Quantize_MAX, H264_Quantize_MIN, H264_Idrframe_MAX, H264_Idrframe_MIN, 
#else
			-1, -1, -1, -1,
#endif
#ifdef VIDEO_CODEC_JPEG
			JPEG_Quantize_MAX, JPEG_Quantize_MIN);
#else
			-1, -1);
#endif
}

static void SSI_entry_video(char *pcInsert, int iInsertLen)
{
	int outlen = snprintf(pcInsert, iInsertLen, "\n\tvenccount=%u;\n\trtspport=%u;\n", VIDEO_CHANNEL_CNT, gtNetwork.portnumRTSP);
	pcInsert += outlen;
	iInsertLen -= outlen;

	char *item = "\tvar arr%s = [";

	outlen = snprintf(pcInsert, iInsertLen, item, "Codec");
	pcInsert += outlen;	iInsertLen -= outlen;
	for (int i = 0; i < VIDEO_CHANNEL_CNT; i++) {
		VideoChannel *vinfo = (VideoChannel *)&gtUser.vcVideo[i];
		outlen = snprintf(pcInsert, iInsertLen, "%d, ", vinfo->eCodec);
		pcInsert += outlen;	iInsertLen -= outlen;
	}
	outlen = snprintf(pcInsert, iInsertLen, "];\n");
	pcInsert += outlen;	iInsertLen -= outlen;

	outlen = snprintf(pcInsert, iInsertLen, item, "Profile");
	pcInsert += outlen;	iInsertLen -= outlen;
	for (int i = 0; i < VIDEO_CHANNEL_CNT; i++) {
		VideoChannel *vinfo = (VideoChannel *)&gtUser.vcVideo[i];
		outlen = snprintf(pcInsert, iInsertLen, "%d, ", vinfo->eProfileMode);
		pcInsert += outlen;	iInsertLen -= outlen;
	}
	outlen = snprintf(pcInsert, iInsertLen, "];\n");
	pcInsert += outlen;	iInsertLen -= outlen;

	outlen = snprintf(pcInsert, iInsertLen, item, "Resolution");
	pcInsert += outlen;	iInsertLen -= outlen;
	for (int i = 0; i < VIDEO_CHANNEL_CNT; i++) {
		VideoChannel *vinfo = (VideoChannel *)&gtUser.vcVideo[i];
		outlen = snprintf(pcInsert, iInsertLen, "%d, ", vinfo->eResolution);
		pcInsert += outlen;	iInsertLen -= outlen;
	}
	outlen = snprintf(pcInsert, iInsertLen, "];\n");
	pcInsert += outlen;	iInsertLen -= outlen;

	outlen = snprintf(pcInsert, iInsertLen, item, "Idrframe");
	pcInsert += outlen;	iInsertLen -= outlen;
	for (int i = 0; i < VIDEO_CHANNEL_CNT; i++) {
		VideoChannel *vinfo = (VideoChannel *)&gtUser.vcVideo[i];
		outlen = snprintf(pcInsert, iInsertLen, "%d, ", vinfo->nIDRFrame);
		pcInsert += outlen;	iInsertLen -= outlen;
	}
	outlen = snprintf(pcInsert, iInsertLen, "];\n");
	pcInsert += outlen;	iInsertLen -= outlen;

	outlen = snprintf(pcInsert, iInsertLen, item, "Quality");
	pcInsert += outlen;	iInsertLen -= outlen;
	for (int i = 0; i < VIDEO_CHANNEL_CNT; i++) {
		VideoChannel *vinfo = (VideoChannel *)&gtUser.vcVideo[i];
		outlen = snprintf(pcInsert, iInsertLen, "%d, ", vinfo->nQuality);
		pcInsert += outlen;	iInsertLen -= outlen;
	}
	outlen = snprintf(pcInsert, iInsertLen, "];\n");
	pcInsert += outlen;	iInsertLen -= outlen;

	outlen = snprintf(pcInsert, iInsertLen, item, "Fps");
	pcInsert += outlen;	iInsertLen -= outlen;
	for (int i = 0; i < VIDEO_CHANNEL_CNT; i++) {
		VideoChannel *vinfo = (VideoChannel *)&gtUser.vcVideo[i];
		outlen = snprintf(pcInsert, iInsertLen, "%d, ", vinfo->nFps);
		pcInsert += outlen;	iInsertLen -= outlen;
	}
	outlen = snprintf(pcInsert, iInsertLen, "];\n");
	pcInsert += outlen;	iInsertLen -= outlen;

	outlen = snprintf(pcInsert, iInsertLen, item, "Brmode");
	pcInsert += outlen;	iInsertLen -= outlen;
	for (int i = 0; i < VIDEO_CHANNEL_CNT; i++) {
		VideoChannel *vinfo = (VideoChannel *)&gtUser.vcVideo[i];
		outlen = snprintf(pcInsert, iInsertLen, "%d, ", vinfo->eBRMode);
		pcInsert += outlen;	iInsertLen -= outlen;
	}
	outlen = snprintf(pcInsert, iInsertLen, "];\n");
	pcInsert += outlen;	iInsertLen -= outlen;

	outlen = snprintf(pcInsert, iInsertLen, item, "Brate");
	pcInsert += outlen;	iInsertLen -= outlen;
	for (int i = 0; i < VIDEO_CHANNEL_CNT; i++) {
		VideoChannel *vinfo = (VideoChannel *)&gtUser.vcVideo[i];
		outlen = snprintf(pcInsert, iInsertLen, "%d, ", vinfo->eBitRate);
		pcInsert += outlen;	iInsertLen -= outlen;
	}
	outlen = snprintf(pcInsert, iInsertLen, "];\n");
	pcInsert += outlen;	iInsertLen -= outlen;
}

static void SSI_entry_play(char *pcInsert, int iInsertLen)
{
	printf("%s\n", __func__);
#if 1
	char *strOutput = "\n"
		"var ip='%IP';\n"
		"var ip_ap='%IP';\n"
		"var ip_sta='%IP';\n"
		"var rtpport=%u;\n"
		"var systime=%u;\n"
		"var ID='%s';\n"
		"var PW='%s';\n";

	es_snprintf(pcInsert, iInsertLen, strOutput,
#if defined(__ETHERNET__)
		gtNetwork.naEthernet.u32IpAddr,
#else
		0, 
#endif
#if defined(__WIFI__)
		gtNetwork.UAPcfg.un32Localip, gtNetwork.STAcfg.un32Localip, 
#else
		0, 0, 
#endif
		gtNetwork.portnumRTSP, gptMsgShare.TIME, gtNetwork.uldInfo[0].strUserID, "[NULL]");
#endif
}

static void SSI_entry_info_m(char *pcInsert, int iInsertLen)
{
	char *strOutput = "\n"
		"var m_total=%u;\n"
		"var m_free1=%u;\n"
		"var m_free2=%u;\n";

// Memory infomation
	UINT m_total = xPortGetTotalHeapSize();
	UINT m_free1 = xPortGetFreeHeapSize();
	UINT m_free2 = xPortGetMinimumEverFreeHeapSize();

// Output
	snprintf(pcInsert, iInsertLen, strOutput, m_total, m_free1, m_free2);
}

static void SSI_entry_info_t(char *pcInsert, int iInsertLen)
{
	printf("%s\n", __func__);

	char *strOutput = "\n"
		"var t_count=%u;\n"
		"var t_string=' TaskID  Task      State   Priority  Remained/Stack    RunTimeCounter  Percentage\\n--------------------------------------------------------------------------------\\n%s';\n";

// FreeRTOS Task Info
	const char * const task_state[5] = {"   Run   ", "  Ready  ", " Blocked ", "Suspended", " Deleted "};
	uint32_t uiTotal = 0;
	UBaseType_t uxTask = 0;
	//TaskStatus_t etiList[TaskStatusCount];
	TaskStatus_t *etiList;
	UBaseType_t TaskStatusCount = uxTaskGetNumberOfTasks();
	etiList = pvPortCalloc(sizeof(TaskStatus_t), TaskStatusCount);

	uxTask = uxTaskGetSystemState(etiList, TaskStatusCount, &uiTotal);

	int nPos = 0;
	char *pbuf = pvPortMalloc((TaskStatusCount+1)*84);

	for (UBaseType_t i = 0; i < TaskStatusCount; i++) {
		nPos += sprintf(pbuf + nPos, " %-6lu %-8s %-9s    %lu/%lu    %4u/0x%08X   %1u      %-u%%\\n",
				etiList[i].xTaskNumber,
				etiList[i].pcTaskName,
				task_state[etiList[i].eCurrentState],
				etiList[i].uxCurrentPriority,
				etiList[i].uxBasePriority,
				etiList[i].usStackHighWaterMark,
				(UINT)(intptr_t)etiList[i].pxStackBase,
				etiList[i].ulRunTimeCounter,
				(etiList[i].ulRunTimeCounter * 100) / uiTotal
				);
	}
	nPos += sprintf(pbuf + nPos, "----------------------------------------------TotalRunTime(%10u)------------", uiTotal);
	vPortFree(etiList);

// Output
	snprintf(pcInsert, iInsertLen, strOutput, 
		TaskStatusCount,
		pbuf);

	vPortFree(pbuf);
}

#if (ENX_RTSP_use==1)
#include "rtspd.h"
static void SSI_entry_info_r(char *pcInsert, int iInsertLen)
{
	printf("%s\n", __func__);

	char *strOutput = "\n"
		"var stm_cnt=[%u,%u];\n";

// RTSP stream user
	int i;
	struct tm tmout;
	char strTemp[RTSP_info_MAX][256] = {{0}};
	int nTempSize[RTSP_info_MAX] = {0};
	char strStmUser[256] = "var stm_user=[", strBuff[16] = {0};	
	char *strPos = strStmUser;
	for (i = 0; i < RTSP_info_MAX; i++) {
		sprintf(strBuff, "stm_user%d,", i);
		strPos = strcat(strPos, strBuff);
		rtsp_connect_info *conn_info = rtspd_connect_info_get_index(i);
		if (conn_info->connect_time != 0) {
			enx_get_tmtime(conn_info->connect_time, &tmout, ENX_YES);
			nTempSize[i] = es_snprintf(strTemp[i], 256, "var stm_user%d=['%IP:%d','%IP:%d','%s','%04d-%02d-%02d %02d:%02d:%02d','%s'];\n", i,
				conn_info->server_addr, conn_info->server_port,
				conn_info->client_addr, conn_info->client_port,
				conn_info->rtp_type == 1 ? "UDP" : conn_info->rtp_type == 2 ? "TCP" : conn_info->rtp_type == 3 ? "HTTP" : "Unknown",
				tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, tmout.tm_hour, tmout.tm_min, tmout.tm_sec,
				conn_info->video_type == 1 ? "H.264 1st" : conn_info->video_type == 2 ? "H.264 2nd" : conn_info->video_type == 3 ? "Motion JPEG" : conn_info->video_type == 0 ? "H.264 file" : "Unknown");
		} else {
			nTempSize[i] = snprintf(strTemp[i], 256, "var stm_user%d=['','','','',''];\n", i);
		}
	}
	strPos = strcat(strPos, "];\n");

// Output
	snprintf(pcInsert, iInsertLen, strOutput, 
			RTSP_info_MAX, rtspd_connect_info_count());

	strPos = pcInsert;
	for (i = 0; i < RTSP_info_MAX; i++) {
		strPos = strcat(strPos, strTemp[i]);
	}
	strcat(strPos, strStmUser);
}
#endif

//static int update_ing = -1;
static void SSI_entry_uping(char *pcInsert, int iInsertLen)
{
	printf("%s\n", __func__);
#if 0
	int update_ing = fwupdate_process();
#else
	int update_ing = -1;
#endif
	printf("[%d%%]", update_ing);
	snprintf(pcInsert, iInsertLen, "\n\tnowver='%x.%x.%x';\n\tuping=%d;\n\t", ((gtSystem.u32Version &0xf00)>>8), ((gtSystem.u32Version &0x0f0)>>4), ((gtSystem.u32Version &0x00f)>>0), update_ing);
}

static void SSI_entry_sdformat(char *pcInsert, int iInsertLen)
{
	int update_ing = SDcardGetFormatState();
	printf("[%d]", update_ing);
	snprintf(pcInsert, iInsertLen, "\n\tsdfmt=%d;\n", update_ing);
}

int App_EN675SSIHandler(int iIndex, char *pcInsert, int iInsertLen)
{
	switch(iIndex)
	{
		case SSI_ENTR_state:
			SSI_entry_state(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_func:
			SSI_entry_func(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_tcp:
			SSI_entry_tcp(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_upnp:
			SSI_entry_upnp(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_vinfo:
			SSI_entry_vinfo(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_sdformat:
			SSI_entry_sdformat(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_user:
			SSI_entry_user(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_date:
			SSI_entry_date(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_video:
			SSI_entry_video(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_play:
			SSI_entry_play(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_info_m:
			SSI_entry_info_m(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_info_t:
			SSI_entry_info_t(pcInsert, iInsertLen);
			break;
#if (ENX_RTSP_use==1)
		case SSI_ENTR_info_r:
			SSI_entry_info_r(pcInsert, iInsertLen);
			break;
#endif
		case SSI_ENTR_uping:
			SSI_entry_uping(pcInsert, iInsertLen);
			break;

		default:
			return ENX_FAIL;
	}
	return ENX_OK;
}
#endif

#if LWIP_HTTPD_CGI

static const char strUrl_reboot_cmd[] = "/system_reboot_cmd.html";
static const char strUrl_reboot_dly[] = "/system_reboot_ing.html";
static const char strUrl_submit[] = "/submit.html";
static const char strUrl_405[] = "/405.html";
static const char strUrl_uping[] = "/system_uping.html";
static const char strUrl_netchange1[] = "/network_submit1.html";
static const char strUrl_netchange2[] = "/network_submit2.html";
static const char strUrl_snap[] = "/still.jpg";
static const char strUrl_sdnotready[] = "/sdcard_notready.html";
static const char strUrl_sdnotsave[] = "/sdcard_notsave.html";
static const char strUrl_sdformat[] = "/sdcard_formating.html";

static uint32 un32GetValue = 0;
static uint32 un32TempValue;

#if 0
typedef struct {
	char *strRate;
	UINT unIndex;
	VideoBitRate sbrRate;
} CBRrate;

CBRrate cbrValue[] = {
	{"hC20m", e_hc20m, e_rate20mbps},
	{"hC19m", e_hc19m, e_rate19mbps},
	{"hC18m", e_hc18m, e_rate18mbps},
	{"hC17m", e_hc17m, e_rate17mbps},
	{"hC16m", e_hc16m, e_rate16mbps},
	{"hC15m", e_hc15m, e_rate15mbps},
	{"hC14m", e_hc14m, e_rate14mbps},
	{"hC13m", e_hc13m, e_rate13mbps},
	{"hC12m", e_hc12m, e_rate12mbps},
	{"hC11m", e_hc11m, e_rate11mbps},
	{"hC10m", e_hc10m, e_rate10mbps},
	{"hC9m", e_hc9m, e_rate9mbps},
	{"hC8m", e_hc8m, e_rate8mbps},
	{"hC7m", e_hc7m, e_rate7mbps},
	{"hC6m", e_hc6m, e_rate6mbps},
	{"hC5m", e_hc5m, e_rate5mbps},
	{"hC4m", e_hc4m, e_rate4mbps},
	{"hC3m", e_hc3m, e_rate3mbps},
	{"hC2m", e_hc2m, e_rate2mbps},
	{"hC1m", e_hc1m, e_rate1mbps},
	{"hC512k", e_hc512k, e_rate512kbps},
	{"hC256k", e_hc256k, e_rate256kbps},
	{"hC128k", e_hc128k, e_rate128kbps},

	{"jC50m", e_jc50m, e_rate50mbps},
	{"jC45m", e_jc45m, e_rate45mbps},
	{"jC40m", e_jc40m, e_rate40mbps},
	{"jC35m", e_jc35m, e_rate35mbps},
	{"jC30m", e_jc30m, e_rate30mbps},
	{"jC25m", e_jc25m, e_rate25mbps},
	{"jC20m", e_jc20m, e_rate20mbps},
	{"jC19m", e_jc19m, e_rate19mbps},
	{"jC18m", e_jc18m, e_rate18mbps},
	{"jC17m", e_jc17m, e_rate17mbps},
	{"jC16m", e_jc16m, e_rate16mbps},
	{"jC15m", e_jc15m, e_rate15mbps},
	{"jC14m", e_jc14m, e_rate14mbps},
	{"jC13m", e_jc13m, e_rate13mbps},
	{"jC12m", e_jc12m, e_rate12mbps},
	{"jC11m", e_jc11m, e_rate11mbps},
	{"jC10m", e_jc10m, e_rate10mbps},
	{"jC9m", e_jc9m, e_rate9mbps},
	{"jC8m", e_jc8m, e_rate8mbps},
	{"jC7m", e_jc7m, e_rate7mbps},
	{"jC6m", e_jc6m, e_rate6mbps},
	{"jC5m", e_jc5m, e_rate5mbps},
	{"jC4m", e_jc4m, e_rate4mbps},
	{"jC3m", e_jc3m, e_rate3mbps},
	{"jC2m", e_jc2m, e_rate2mbps},
	{"jC1m", e_jc1m, e_rate1mbps}
};
#endif

#if defined(__ETHERNET__)
static ENX_OKFAIL CGI_entry_IPCHANG(UINT *ipaddr, char *pcValue, UINT UIN_MSGTYPE)
{
	UINT bRes = ENX_FAIL;
	un32TempValue = ipaddr_addr(pcValue);
	if (un32TempValue == IPADDR_NONE) {
		printf("ERROR %s\n", __func__);
	} else {
		if (*ipaddr != un32TempValue) {
			*ipaddr = un32TempValue;
			un32GetValue = bitchange(un32GetValue, UIN_MSGTYPE, 1);
			bRes = ENX_OK;
		}
	}
	return bRes;
}
#endif

static ENX_OKFAIL CGI_entry_INTEGER_MAX_MIN(UINT *unInterger, char *pcValue, UINT UIN_MSGTYPE, UINT nMax, UINT nMin)
{
	ENX_OKFAIL bRes = ENX_FAIL;
	un32TempValue = atoi(pcValue);
	if (*unInterger != un32TempValue) {
		if (nMax < nMin) {
			UINT nTemp = nMax;
			nMax = nMin;
			nMin = nTemp;
		}
		if (un32TempValue < nMin)		un32TempValue = nMin;
		else if (un32TempValue > nMax)	un32TempValue = nMax;
		*unInterger = un32TempValue;
		un32GetValue = bitchange(un32GetValue, UIN_MSGTYPE, 1);
		bRes = ENX_OK;
	}
	return bRes;
}

#if defined(__ETHERNET__)
static ENX_OKFAIL CGI_entry_HEXINTEGER8_MAX_MIN(BYTE *unInterger, char *pcValue, UINT UIN_MSGTYPE, BYTE nMax, BYTE nMin)
{
	ENX_OKFAIL bRes = ENX_FAIL;
	BYTE un8TempValue = strtol(pcValue, NULL, 16);
	if (*unInterger != un8TempValue) {
		if (nMax < nMin) {
			UINT nTemp = nMax;
			nMax = nMin;
			nMin = nTemp;
		}
		if (un8TempValue < nMin)		un8TempValue = nMin;
		else if (un8TempValue > nMax)	un8TempValue = nMax;
		*unInterger = un8TempValue;
		un32GetValue = bitchange(un32GetValue, UIN_MSGTYPE, 1);
		bRes = ENX_OK;
	}
	return bRes;
}
#endif

static UINT CGI_entry_SWITCH(UINT *bSwitch, char *pcValue, UINT UIN_MSGTYPE)
{
	return CGI_entry_INTEGER_MAX_MIN(bSwitch, pcValue, UIN_MSGTYPE, 1, 0);
}

static UINT CGI_entry_STRING(char *strString, char *pcValue, UINT UIN_MSGTYPE)
{
	ENX_OKFAIL bRes = ENX_FAIL;
	if (strcmp(strString, pcValue) != 0) {
		strcpy(strString, pcValue);
		un32GetValue = bitchange(un32GetValue, UIN_MSGTYPE, 1);
		bRes = ENX_OK;
	}
	return bRes;
}

#if defined(__JPEG__) || defined(__H264__)
static UINT CGI_entry_BRRATE(UINT *unInterger, char *pcValue, UINT UIN_MSGTYPE)
{
	ENX_OKFAIL bRes = ENX_FAIL;
	for (UINT i = 0; i < (sizeof(cbrValue) / sizeof(cbrValue[0])); i++) {
		if (strcmp(pcValue, cbrValue[i].strRate) == 0) {
			if (*unInterger != cbrValue[i].unIndex) {
				*unInterger = cbrValue[i].unIndex;
				un32GetValue = bitchange(un32GetValue, UIN_MSGTYPE, 1);
				bRes = ENX_OK;
				break;
			}
		}
	}
	return bRes;
}
#endif
UINT unVideoChangeFlag = 0;
const char *App_EN675CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	char strUserPWChange[32 + 1] = {0};
	int i, nUserPWChange = 0;
	UINT nYear = 0, nMonth = 0, nDay = 0, nHour = 0, nMin = 0, nSec = 0;	// Datetime
	un32GetValue = 0;
	printf("==============================================================\n");
	printf("call %s\n", __func__);
#if defined(__ETHERNET__)
	UINT unEthernetIpChangeFlag = 0;
#endif
	unVideoChangeFlag = 0;

	for (i = 0; i < iNumParams; i++) {
		un32TempValue = 0;
		if(strcmp(pcParam[i], "btOK") == 0)	continue;
		printf("%s : %s \n", pcParam[i], pcValue[i]);
		///////////////////////////////////////////////////////////////
		// TCP ////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
#if defined(__ETHERNET__)
		if(strcmp(pcParam[i], "n_t_ip") == 0)
		{
			if(CGI_entry_IPCHANG((UINT *)&(gtNetwork.naEthernet.u32IpAddr), pcValue[i], UIN_OK) == ENX_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_IP;
		}
		else if(strcmp(pcParam[i], "n_t_sn") == 0)
		{
			if(CGI_entry_IPCHANG((UINT *)&(gtNetwork.naEthernet.u32NetMask), pcValue[i], UIN_OK) == ENX_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_SN;
		}
		else if(strcmp(pcParam[i], "n_t_gw") == 0)
		{
			if(CGI_entry_IPCHANG((UINT *)&(gtNetwork.naEthernet.u32Gateway), pcValue[i], UIN_OK) == ENX_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_GW;
		}
		else if(strcmp(pcParam[i], "n_t_dns0") == 0)
		{
			if(CGI_entry_IPCHANG((UINT *)&(gtNetwork.naEthernet.u32DnsSvr0), pcValue[i], UIN_OK) == ENX_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_DNS0;
		}
		else if(strcmp(pcParam[i], "n_t_dns1") == 0)
		{
			if(CGI_entry_IPCHANG((UINT *)&(gtNetwork.naEthernet.u32DnsSvr1), pcValue[i], UIN_OK) == ENX_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_DNS1;
		}
		else if(strcmp(pcParam[i], "n_t_dhcp") == 0)
		{
			if(CGI_entry_SWITCH((UINT *)&(gtNetwork.naEthernet.u1UseDhcp), pcValue[i], UIN_OK) == ENX_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_DHCP;
		}
		///////////////////////////////////////////////////////////////
		// MAC ////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "n_t_mac0") == 0)
		{
			if(CGI_entry_HEXINTEGER8_MAX_MIN((BYTE *)&(gtSystem.arr8MacAddress[0]), pcValue[i], UIN_OK, 0xFF, 0) == ENX_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_MAC;
		}
		else if(strcmp(pcParam[i], "n_t_mac1") == 0)
		{
			if(CGI_entry_HEXINTEGER8_MAX_MIN((BYTE *)&(gtSystem.arr8MacAddress[1]), pcValue[i], UIN_OK, 0xFF, 0) == ENX_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_MAC;
		}
		else if(strcmp(pcParam[i], "n_t_mac2") == 0)
		{
			if(CGI_entry_HEXINTEGER8_MAX_MIN((BYTE *)&(gtSystem.arr8MacAddress[2]), pcValue[i], UIN_OK, 0xFF, 0) == ENX_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_MAC;
		}
		else if(strcmp(pcParam[i], "n_t_mac3") == 0)
		{
			if(CGI_entry_HEXINTEGER8_MAX_MIN((BYTE *)&(gtSystem.arr8MacAddress[3]), pcValue[i], UIN_OK, 0xFF, 0) == ENX_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_MAC;
		}
		else if(strcmp(pcParam[i], "n_t_mac4") == 0)
		{
			if(CGI_entry_HEXINTEGER8_MAX_MIN((BYTE *)&(gtSystem.arr8MacAddress[4]), pcValue[i], UIN_OK, 0xFF, 0) == ENX_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_MAC;
		}
		else if(strcmp(pcParam[i], "n_t_mac5") == 0)
		{
			if(CGI_entry_HEXINTEGER8_MAX_MIN((BYTE *)&(gtSystem.arr8MacAddress[5]), pcValue[i], UIN_OK, 0xFF, 0) == ENX_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_MAC;
		}
		else 
#endif
		///////////////////////////////////////////////////////////////
		// UPNP ///////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		if(strcmp(pcParam[i], "n_u_id") == 0)			CGI_entry_STRING((char *)gtUser.strDeviceId, pcValue[i], UIN_OK);
		else if(strcmp(pcParam[i], "n_u_name") == 0)	CGI_entry_STRING((char *)gtUser.strDeviceName, pcValue[i], UIN_OK);
		///////////////////////////////////////////////////////////////
		// RTSP PORT //////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "n_r_port") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtNetwork.portnumRTSP), pcValue[i], UIN_OK, 0xFFFF, 0) == ENX_OK)
				unVideoChangeFlag |= UIN_STREAM_PORT;
		}
		///////////////////////////////////////////////////////////////
		// USER ///////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////		
#if 0
		else if(strcmp(pcParam[i], "s_u_id") == 0)
		{
			CGI_entry_STRING((char *)gtNetwork.strUserId, pcValue[i], UIN_OK);
		}
		else if(strcmp(pcParam[i], "s_u_pw") == 0)
		{
			strncpy(strUserPWChange, pcValue[i], 32);
			nUserPWChange = 1;
//			CGI_entry_STRING((char *)gtNetwork.strUserPw, pcValue[i], UIN_OK);
		}
#endif
#if defined(__FILESYSTEM__)
#if (ENX_FTPD_use==1)
		else if(strcmp(pcParam[i], "s_u_ftp") == 0)
		{
			CGI_entry_SWITCH((UINT *)&(gtNetwork.bFtpServer), pcValue[i], UIN_UR_FTP);
		}
#endif
#endif
#ifdef __AUDIO__
#if 0
		else if(strcmp(pcParam[i], "s_u_aud") == 0)
		{
			if(CGI_entry_SWITCH((UINT *)&(gtUser.bAudio), pcValue[i], UIN_OK) == ENX_OK)
				unVideoChangeFlag |= UIN_STREAM_AUDIO;
		}
#endif
#endif
		///////////////////////////////////////////////////////////////
		// SNTP ///////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
#if (ENX_SNTPC_use==1)
		else if(strcmp(pcParam[i], "s_s_svw") == 0)
		{
			CGI_entry_SWITCH((UINT *)&(gtNetwork.bUseSntp), pcValue[i], UIN_UR_SNTP_SW);
		}
		else if(strcmp(pcParam[i], "s_s_svr") == 0)
		{
			CGI_entry_STRING((char *)gtNetwork.strSntpSvr, pcValue[i], UIN_UR_SNTP_SW);
		}
		else if(strcmp(pcParam[i], "s_s_svp") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtNetwork.nSntpPeriodic), pcValue[i], UIN_UR_SNTP_SW, 0xFFFFFFFF, 15);
		}
		else if(strcmp(pcParam[i], "s_s_svs") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtNetwork.nSntpRetrySec), pcValue[i], UIN_UR_SNTP_SW, 0xFFFFFFFF, 3);
		}
		else if(strcmp(pcParam[i], "s_s_svm") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtNetwork.nSntpRetryMaxcnt), pcValue[i], UIN_UR_SNTP_SW, 0xFFFFFFFF, 0);
		}
#endif
		///////////////////////////////////////////////////////////////
		// DATETIME ///////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "s_s_uy") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(nYear), pcValue[i], UIN_UR_DATETIME, 2100, 1970);
		}
		else if(strcmp(pcParam[i], "s_s_um") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(nMonth), pcValue[i], UIN_UR_DATETIME, 12, 1);
		}
		else if(strcmp(pcParam[i], "s_s_ud") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(nDay), pcValue[i], UIN_UR_DATETIME, 31, 1);
		}
		else if(strcmp(pcParam[i], "s_s_uh") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(nHour), pcValue[i], UIN_UR_DATETIME, 23, 0);
		}
		else if(strcmp(pcParam[i], "s_s_ui") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(nMin), pcValue[i], UIN_UR_DATETIME, 59, 0);
		}
		else if(strcmp(pcParam[i], "s_s_us") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(nSec), pcValue[i], UIN_UR_DATETIME, 59, 0);
		}
		else if(strcmp(pcParam[i], "s_s_st") == 0)
		{
			CGI_entry_SWITCH((UINT *)&(gtUser.nTimeSummer), pcValue[i], UIN_UR_TIMEZONE);
		}
		else if(strcmp(pcParam[i], "s_s_tz") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.nTimeZone), pcValue[i], UIN_UR_TIMEZONE, e_tzP1400, e_tzM1200);
		}
		///////////////////////////////////////////////////////////////
		// VIDEO //////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
#ifdef __JPEG__
		else if(strcmp(pcParam[i], "v_v_jr") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviJpeg.eResolution), pcValue[i], UIN_OK, e_resEndorUnknown - 1, IMG_JPEG_RES) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_JPEG_SIZE;
		}
		else if(strcmp(pcParam[i], "v_v_jq") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviJpeg.nQuality), pcValue[i], UIN_OK, JPEG_Quantize_MAX, JPEG_Quantize_MIN) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_JPEG_QUALITY;
		}
		else if(strcmp(pcParam[i], "v_v_jf") == 0)
		{
			ENX_OKFAIL bRes = ENX_FAIL;
			switch(gptMsgShare.VIDEO_FPS)
			{
				case 60:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviJpeg.sfFPS), pcValue[i], UIN_OK, e_s60f1, 0);
					break;
				case 50:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviJpeg.sfFPS), pcValue[i], UIN_OK, e_s50f1, 0);
					break;
				case 30:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviJpeg.sfFPS), pcValue[i], UIN_OK, e_s30f1, 0);
					break;
				case 25:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviJpeg.sfFPS), pcValue[i], UIN_OK, e_s25f1, 0);
					break;
			}
			if(bRes == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_JPEG_FPS;
		}
		else if(strcmp(pcParam[i], "v_v_jbm") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT*)&(gtUser.uviJpeg.eBRMode), pcValue[i], UIN_OK, e_brmCBR, e_brmQBR) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_JPEG_BRMODE;
		}
		else if(strcmp(pcParam[i], "v_v_jbr") == 0)
		{
			if(CGI_entry_BRRATE((UINT *)&(gtUser.uviJpeg.nBitRate), pcValue[i], UIN_OK) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_JPEG_BRATE;
		}
#endif
#ifdef __H264__
		else if(strcmp(pcParam[i], "v_v_hp") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].eProfileMode), pcValue[i], UIN_OK, e_pmMainCB, e_pmBaseline) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_1CH_PROFILE;
		}
		else if(strcmp(pcParam[i], "v_v_hr") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].eResolution), pcValue[i], UIN_OK, e_resEndorUnknown - 1, IMG_H264_RES) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_1CH_SIZE;
		}
		else if(strcmp(pcParam[i], "v_v_hq") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].nQuality), pcValue[i], UIN_OK, H264_Quantize_MAX, H264_Quantize_MIN) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_1CH_QUALITY;
		}
		else if(strcmp(pcParam[i], "v_v_hi") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].nIDRFrame), pcValue[i], UIN_OK, H264_Idrframe_MAX, H264_Idrframe_MIN) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_1CH_IDRFRAME;
		}
		else if(strcmp(pcParam[i], "v_v_hf") == 0)
		{
			ENX_OKFAIL bRes = ENX_FAIL;
			switch(gptMsgShare.VIDEO_FPS)
			{
				case 60:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].sfFPS), pcValue[i], UIN_OK, e_s60f1, 0);
					break;
				case 50:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].sfFPS), pcValue[i], UIN_OK, e_s50f1, 0);
					break;
				case 30:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].sfFPS), pcValue[i], UIN_OK, e_s30f1, 0);
					break;
				case 25:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].sfFPS), pcValue[i], UIN_OK, e_s25f1, 0);
					break;
			}
#if (model_2M)
			if(MP(FrameMode) == 3 || MP(FrameMode) == 5)	// 50/60 fps(WDR이 아닌 상태)
			{
				if(gtUser.uviH264[0].eResolution == e_res1920x1080)
				{	// 0ch이 FHD인 상태에서0ch이 max fps만큼H.264 인코딩을 할 수 없다.
					if(gtUser.uviH264[0].sfFPS == 0)
						gtUser.uviH264[0].sfFPS = 1;
				}
			}
#endif
			if(bRes == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_1CH_FPS;
		}
		else if(strcmp(pcParam[i], "v_v_hbm") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT*)&(gtUser.uviH264[0].eBRMode), pcValue[i], UIN_OK, e_brmCVBR, e_brmQBR) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_1CH_BRMODE;
		}
		else if(strcmp(pcParam[i], "v_v_hbr") == 0)
		{
			if(CGI_entry_BRRATE((UINT *)&(gtUser.uviH264[0].nBitRate), pcValue[i], UIN_OK) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_1CH_BRATE;
		}
#ifdef __H264_2CH__
		else if(strcmp(pcParam[i], "v_v_h2p") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].eProfileMode), pcValue[i], UIN_OK, e_pmMainCB, e_pmBaseline) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_2CH_PROFILE;
		}
		else if(strcmp(pcParam[i], "v_v_h2r") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].eResolution), pcValue[i], UIN_OK, e_resEndorUnknown - 1, IMG_H264_2CH_RES) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_2CH_SIZE;
		}
		else if(strcmp(pcParam[i], "v_v_h2q") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].nQuality), pcValue[i], UIN_OK, H264_Quantize_MAX, H264_Quantize_MIN) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_2CH_QUALITY;
		}
		else if(strcmp(pcParam[i], "v_v_h2i") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].nIDRFrame), pcValue[i], UIN_OK, H264_Idrframe_MAX, H264_Idrframe_MIN) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_2CH_IDRFRAME;
		}
		else if(strcmp(pcParam[i], "v_v_h2f") == 0)
		{
			ENX_OKFAIL bRes = ENX_FAIL;
			switch(gptMsgShare.VIDEO_FPS)
			{
				case 60:
					bRes =CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].sfFPS), pcValue[i], UIN_OK, e_s60f1, 0);
					break;
				case 50:
					bRes =CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].sfFPS), pcValue[i], UIN_OK, e_s50f1, 0);
					break;
				case 30:
					bRes =CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].sfFPS), pcValue[i], UIN_OK, e_s30f1, 0);
					break;
				case 25:
					bRes =CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].sfFPS), pcValue[i], UIN_OK, e_s25f1, 0);
					break;
			}
#if (model_2M)
			if(MP(FrameMode) == 3 || MP(FrameMode) == 5)	// 50/60 fps(WDR이 아닌 상태)
			{
				if(gtUser.uviH264[0].eResolution == e_res1920x1080)
				{	// 0ch이 FHD인 상태에서1ch이 max fps만큼H.264 인코딩을 할 수 없다.
					if(gtUser.uviH264[1].sfFPS == 0)
						gtUser.uviH264[1].sfFPS = 1;
				}
			}
#endif
			if(bRes == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_2CH_FPS;
		}
		else if(strcmp(pcParam[i], "v_v_h2bm") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT*)&(gtUser.uviH264[1].eBRMode), pcValue[i], UIN_OK, e_brmCVBR, e_brmQBR) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_2CH_BRMODE;
		}
		else if(strcmp(pcParam[i], "v_v_h2br") == 0)
		{
			if(CGI_entry_BRRATE((UINT *)&(gtUser.uviH264[1].nBitRate), pcValue[i], UIN_OK) == ENX_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_2CH_BRATE;
		}
#endif
#endif
		else if (strncmp(pcParam[i], "s_i_kik", 7) == 0) {
			int nUserIdx = -1;
			if (sscanf(pcParam[i], "s_i_kik%d", &nUserIdx) == 1) {
				rtspd_connect_kick(nUserIdx);
			} else {
				printf("Kick idx error(%d)\n", nUserIdx);
			}
		} else {
			printf("Error Msg : %s(%s)\n", pcParam[i], pcValue[i]);
		}
	}

	if (nUserPWChange == 1) {
//		UserPasswordSet(strUserPWChange);
	}

	printf("==============================================================\n");

	UINT un32InfoUpdate = un32GetValue;
#if defined(__ETHERNET__)
	UINT un32Check = unEthernetIpChangeFlag;
	if (unEthernetIpChangeFlag != 0) {
		enx_ipchange_proc(enlETHERNET, unEthernetIpChangeFlag);
	}
#endif
#if 0
	if (unVideoChangeFlag != 0) {
		vTaskCreate("vchange", VideoChangeTask, NULL, _16KB_STACK_SIZE, LV5_TASK_PRIO);
	}
#endif
#if (ENX_SNTPC_use==1)
	if (bitcheck(un32InfoUpdate, UIN_UR_SNTP_SW)) {
		un32InfoUpdate = bitchange(un32InfoUpdate, UIN_UR_SNTP_SW, 0);
		if (gtNetwork.bUseSntp == DEF_ON) {
			printf("SNTP ON\n");
			sntpc_start();
		} else {
			printf("SNTP OFF\n");
			sntpc_stop();
		}
	}
#endif
	if (bitcheck(un32InfoUpdate, UIN_UR_TIMEZONE)) {
		un32InfoUpdate = bitchange(un32InfoUpdate, UIN_UR_TIMEZONE, 0);
		setTimeZone(gtUser.nTimeZone);
	}
	if (bitcheck(un32InfoUpdate, UIN_UR_DATETIME)) {
		un32InfoUpdate = bitchange(un32InfoUpdate, UIN_UR_DATETIME, 0);
		set_devicetime(TimeZone_LOC, nYear, nMonth, nDay, nHour, nMin, nSec);
	}
#if (ENX_FTPD_use==1)
	if (bitcheck(un32InfoUpdate, UIN_UR_FTP)) {
		un32InfoUpdate = bitchange(un32InfoUpdate, UIN_UR_FTP, 0);
		if (gtUser.bFtpServer == DEF_ON) {
			printf("FTP ON\n");
			FtpdStart();
		} else {
			printf("FTP OFF\n");
			FtpdStop();
		}
	}
#endif

	UserAreaCmd(sflsc_Save, sfls_USER);
#if defined(__NETWORK__)
	UserAreaCmd(sflsc_Save, sfls_NETWORK);
#endif

#if defined(__ETHERNET__)
	if(bitcheck(un32Check, UIN_NW_INT_IP))
		return strUrl_netchange1;
	else if(bitcheck(un32Check, UIN_NW_INT_DHCP) && gtNetwork.naEthernet.u1UseDhcp == ENX_ON)
		return strUrl_netchange2;
	else if (bitcheck(un32Check, UIN_NW_INT_MAC)) {
		UserAreaCmd(sflsc_Save, sfls_SYSTEM);
		return strUrl_reboot_cmd;
	}
#endif
	return strUrl_submit;

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}

const char *AppUpdate_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	_Rprintf("%s - error\n", __func__);
	printf("==============================================================\n");
	printf("call %s\n", __func__);
#if 0
	FWUPDATE_INFO* fwinfo = (FWUPDATE_INFO*)pvPortCalloc(1, sizeof(FWUPDATE_INFO));
	fwinfo->ddr_addr = (BYTE *)FW_UPDATE_BASE;	// 이미 DDR에 펌웨어 파일이 올라와 있다. 올라온 주소를 저장한다.
	fwinfo->type = eFWT_DDR;			// 이미 DDR에 펌웨어 파일이 올라와 있다.
	fwinfo->is_areachange = DEF_YES;	// fw write 중 전원차단 등을 예방하기 위해 area을 변경 후 write한다.
	fwinfo->is_bootwrite = DEF_NO;		// boot.bin은 write하지 않는다.
	fwinfo->is_autoreboot = DEF_NO;		// CGI처리에 의해 재부팅 할것이므로 자동재부팅 하지 않는다.
	fwinfo->is_malloc = DEF_YES;		// 이 구조체는 malloc 했으므로 free를 위해 선언되었다.
	fwinfo->is_encodingstop = DEF_NO;	// httpd_post_begin에서 encoding stop 명령을 내린다.
	vTaskCreate("fwup", fwupdate_Task, fwinfo, JABBU_STACK_SIZE, LV5_TASK_PRIO);
#endif
	printf("==============================================================\n");
	return strUrl_uping;

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}

const char *AppUpdate2_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	_Rprintf("%s - error\n", __func__);
	printf("==============================================================\n");
	printf("call %s\n", __func__);
#if 0
	FWUPDATE_INFO* fwinfo = (FWUPDATE_INFO*)pvPortCalloc(1, sizeof(FWUPDATE_INFO));
	fwinfo->ddr_addr = (BYTE *)FW_UPDATE_BASE;	// 이미 DDR에 펌웨어 파일이 올라와 있다. 올라온 주소를 저장한다.
	fwinfo->type = eFWT_DDR;			// 이미 DDR에 펌웨어 파일이 올라와 있다.
	fwinfo->is_areachange = DEF_YES;	// fw write 중 전원차단 등을 예방하기 위해 area을 변경 후 write한다.
	fwinfo->is_bootwrite = DEF_NO;		// boot.bin은 write하지 않는다.
	fwinfo->is_autoreboot = DEF_YES;	// ONVIF에 의해 요청이 들어온 것 이므로 자동 재부팅한다.
	fwinfo->is_malloc = DEF_YES;		// 이 구조체는 malloc 했으므로 free를 위해 선언되었다.
	fwinfo->is_encodingstop = DEF_NO;	// httpd_post_begin에서 encoding stop 명령을 내린다.
	vTaskCreate("fwup", fwupdate_Task, fwinfo, JABBU_STACK_SIZE, LV5_TASK_PRIO);
#endif
	printf("==============================================================\n");
	return strUrl_uping;

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}

const char *AppInit_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	int i;
	_Rprintf("%s - error\n", __func__);
	printf("==============================================================\n");
	printf("call %s\n", __func__);

	for(i=0;i<iNumParams;i++)
	{
		printf("%s : %s \n", pcParam[i], pcValue[i]);
	}

//	LoadDefUserInfo();	// cpu0/init.c
	UserAreaCmd(sflsc_Save, sfls_USER);// init info save
	printf("==============================================================\n");
	WdtWaitReboot(500);

	return strUrl_submit;

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}

const char *AppReboot_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	_Rprintf("%s - error\n", __func__);
	printf("==============================================================\n");
	printf("call %s\n", __func__);
	WdtWaitReboot(1000);
	printf("==============================================================\n");
	return strUrl_reboot_dly;

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}

#if (LOAD_FS_SDCARD==1)
extern const char *ghttpuri; // httpd.c
//extern int gWebls_page; // fsfilelist.c
const char *App_EN675sdfilelistCGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
#if 1
	_Rprintf("%s - error\n", __func__);
#else
	int i;
	gWebls_page = 1;
	for (i = 0; i < iNumParams; i++) {
		if (strcmp(pcParam[i], "page") == 0) {
			gWebls_page = atoi(pcValue[i]);
		}
	}
#endif
	return ghttpuri;
	UNUSED(iIndex);
}

#if JPEG_SNAPSHOT
static const char *AppJpegSave(void)
{
#if (JPEG_SDSAVE==1)
	UINT idle_ck = 0;
	switch (getSDState()) {
		case sd_OFF:	// sd카드x <- sd카드 없음을 알린다.
			printf("%s(%d) : sd_OFF state\n", __func__, __LINE__);
			return strUrl_sdnotready;
		case sd_INIT:	// sd카드o, 초기화 중 <- 대기..?
		case sd_READY:
			printf("%s(%d) : sd_INIT/sd_READY state\n", __func__, __LINE__);
			while (getSDState() == sd_INIT || getSDState() == sd_READY) {
				idle_ck++;
				vTaskDelay(1);
				if(idle_ck > 200)	// idle max 2sec...
					break;
			}
			if(getSDState() != sd_IDLE)
				break;
		case sd_IDLE:	// sd카드o, IDLE <- 즉시 처리 가능
			printf("%s(%d) : sd_IDLE state\n", __func__, __LINE__);
			muxer_jpegstill_request();
			return strUrl_submit;
		case sd_SAVE:	// sd카드o, 녹화 중 <- jpeg save 작업을 videnc로 넘긴다.
			printf("%s(%d) : sd_SAVE state\n", __func__, __LINE__);
			if (muxer_videnc_state(eRecSnapshot) == 0) {
				muxer_videnc_go(eRecSnapshot);
			}
			return strUrl_submit;
		case sd_ERR:	// sd카드o, 오류 <- sd카드 오류 상태임을 알린다.
			printf("%s(%d) : sd_ERR state\n", __func__, __LINE__);
			break;
	}
#else
	printf("%s(%d) : Jpeg snapshot save not supported.\n", __func__, __LINE__);
#endif
	return strUrl_405;
}
#endif

const char *AppSDSetup_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	const char *resPath = NULL;
	int i;
	int flag_sd_save = 0;

	printf("==============================================================\n");
	printf("call %s\n", __func__);

	for (i = 0; i < iNumParams; i++) {
		if(strcmp(pcParam[i], "btOK") == 0) continue;
		printf("%s : %s \n", pcParam[i], pcValue[i]);

		if (strncmp(pcParam[i], "s_u_sd", 6) == 0) {
			CGI_entry_SWITCH((UINT *)&(gtUser.bSdVidSave), pcValue[i], UIN_OK);
			flag_sd_save = 1;
		}
#if JPEG_SNAPSHOT
		else if (strncmp(pcParam[i], "savejpeg", 8) == 0) {
			resPath = AppJpegSave();
		}
#endif
#if (FAT_SDSAVE_EVENT==1)
		else if (strncmp(pcParam[i], "saveevt", 7) == 0) {
			if (getSDState() != sd_OFF) {
				if (gtUser.bSdVidSave == ENX_ON) {
					if (muxer_videnc_state(eRecEvent) == 0) 	{
						muxer_videnc_go(eRecEvent);
					}
					resPath = strUrl_submit;
				} else {
					printf("Error : State => SD Save OFF\n");
					resPath = strUrl_sdnotsave;
				}
			} else {
				resPath = strUrl_sdnotready;
			}
		}
#endif
		else if (strncmp(pcParam[i], "sdformat", 8) == 0) {
			if (getSDState() != sd_OFF) {
				SDcardSetFormat();
				resPath = strUrl_sdformat;
			} else {
				resPath = strUrl_sdnotready;
			}
		} else {
			printf("Error Msg : %s(%s)\n", pcParam[i], pcValue[i]);
		}
	}

	printf("==============================================================\n");

	if (flag_sd_save == 1) {
		UserAreaCmd(sflsc_Save, sfls_USER);
	}

	if (resPath == NULL) {
		return strUrl_submit;
	} else {
		return resPath;
	}

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}
#endif

#if JPEG_SNAPSHOT
const char *AppJpegStill_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
#if JPEG_WEBVIEW
	return strUrl_snap;
#else
	printf("%s(%d) : Jpeg snapshot view not supported.\n", __func__, __LINE__);
	return strUrl_405;
#endif

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}
#endif
#endif
