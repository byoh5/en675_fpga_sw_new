#include "dev.h"

#if defined(__NETWORK__)

#include "enx_lwip.h"
#include "lwip/apps/httpd.h"

#include "cgissi.h"

#if LWIP_HTTPD_SSI

//*****************************************************************************
//!
//! This array holds all the strings that are to be recognized as SSI tag
//! names by the HTTPD server.  The server will call SSIHandler to request a
//! replacement string whenever the pattern <!--#tagname--> (where tagname
//! appears in the following array) is found in ".ssi", ".shtml" or ".shtm"
//! files that it serves.
//!
//*****************************************************************************
static const ConfigSSITags g_pcConfigSSITags[] =
{
// en673 ssi 100 ~ 149
	{SSI_ENTR_state,	"sy_state"},	// Viewer S/W Setting
	{SSI_ENTR_func,		"sy_func"},		// index.html
	{SSI_ENTR_tcp,		"nw_tcp"},		// network_tcp.html
	{SSI_ENTR_upnp,		"nw_upnp"},		// network_upnp.html
	{SSI_ENTR_vinfo,	"vi_info"},		// video_video.html
	{SSI_ENTR_sdformat,	"sd_form"},		// sdcard_setup.html
	{SSI_ENTR_user,		"sy_user"},		// system_user.html
	{SSI_ENTR_date,		"sy_date"},		// system_date.html
	{SSI_ENTR_video,	"vi_video"},	// video_video.html
	{SSI_ENTR_play,		"playinfo"},	// PLAY.html
	{SSI_ENTR_info_m, 	"sy_inf_m"}, 	// system_info.html
	{SSI_ENTR_info_t, 	"sy_inf_t"}, 	// system_info.html
#if (ENX_RTSP_use==1)
	{SSI_ENTR_info_r, 	"sy_inf_r"}, 	// system_info.html
#endif
	{SSI_ENTR_uping,	"sy_uping"},	// system_uping.html

// isp ssi 700 ~ 7xx
#if defined(__ISP__)
	{SSI_ISP_state,		"isp_stat"},
	{SSI_ISP_iris,		"isp_iris"},
	{SSI_ISP_focusadj,	"isp_fo"},
	{SSI_ISP_2motor,	"isp_2mot"},
	{SSI_ISP_exposure,	"isp_ep"},
	{SSI_ISP_backlight,	"isp_back"},
	{SSI_ISP_daynight,	"isp_tdn"},
	{SSI_ISP_color,		"isp_co"},
	{SSI_ISP_image,		"isp_im"},
	{SSI_ISP_motion,	"isp_mo"},
	{SSI_ISP_outs,		"isp_outs"},
	{SSI_ISP_system,	"isp_sy"},
#endif

#if defined(__WIFI__)
	{SSI_WIFI_ap,		"nw_wf_ap"},
	{SSI_WIFI_sta,		"nw_wf_st"},
	{SSI_WIFI_scan,		"nw_wf_sc"},
#endif
};

//*****************************************************************************
//!
//! The number of individual SSI tags that the HTTPD server can expect to
//! find in our configuration pages.
//!
//*****************************************************************************
#define NUM_CONFIG_SSI_TAGS     (sizeof(g_pcConfigSSITags) / sizeof (g_pcConfigSSITags[0]))

//*****************************************************************************
//!
//! Prototype for the main handler used to process server-side-includes for the
//! application's web-based configuration screens.
//!
//! This function is called by the HTTP server whenever it encounters an SSI
//! tag in a web page.  The iIndex parameter provides the index of the tag in
//! the g_pcConfigSSITags array. This function writes the substitution text
//! into the pcInsert array, writing no more than iInsertLen characters.
//!
//*****************************************************************************
static u16_t SSIHandler(int iIndex, char *pcInsert, int iInsertLen)
{
	switch (iIndex) {
		case SSI_ENTR_state:
		case SSI_ENTR_func:
		case SSI_ENTR_tcp:
		case SSI_ENTR_upnp:
		case SSI_ENTR_vinfo:
		case SSI_ENTR_sdformat:
		case SSI_ENTR_user:
		case SSI_ENTR_date:
		case SSI_ENTR_video:
		case SSI_ENTR_play:
		case SSI_ENTR_info_m:
		case SSI_ENTR_info_t:
#if (ENX_RTSP_use==1)
		case SSI_ENTR_info_r:
#endif
		case SSI_ENTR_uping:
			App_EN675SSIHandler(iIndex, pcInsert, iInsertLen);
			break;
#if defined(__ISP__)
		case SSI_ISP_state:
		case SSI_ISP_iris:
		case SSI_ISP_focusadj:
		case SSI_ISP_2motor:
		case SSI_ISP_exposure:
		case SSI_ISP_backlight:
		case SSI_ISP_daynight:
		case SSI_ISP_color:
		case SSI_ISP_outs:
		case SSI_ISP_image:
		case SSI_ISP_motion:
		case SSI_ISP_system:
			App_ISPSSIHandler(iIndex, pcInsert, iInsertLen);
			break;
#endif
#if defined(__WIFI__)
		case SSI_WIFI_ap:
		case SSI_WIFI_sta:
		case SSI_WIFI_scan:
			App_WiFi_SSIHandler(iIndex, pcInsert, iInsertLen);
			break;
#endif
		default:
			snprintf(pcInsert, iInsertLen, " ");
			break;
	}

	return strlen(pcInsert);
}

//*****************************************************************************
//!
//! Initialize the SSI functionality by setting up pointers to
//! the arrays of tags and handlers.
//!
//*****************************************************************************
void ssi_init(void)
{
	// Pass our tag information to the HTTP server.
	http_set_ssi_handler(SSIHandler, g_pcConfigSSITags, NUM_CONFIG_SSI_TAGS);
}
#endif

#if LWIP_HTTPD_CGI
//*****************************************************************************
//!
//! This array is passed to the HTTPD server to inform it of special URIs
//! that are treated as common gateway interface (CGI) scripts.  Each URI name
//! is defined along with a pointer to the function which is to be called to
//! process it.
//! 
//*****************************************************************************
static const tCGI g_psConfigCGIURIs[] =
{
	{ CGIfile_query, App_EN675CGIHandler },
	{ CGIfile_update, AppUpdate_CGIHandler },
	{ CGIfile_update2, AppUpdate2_CGIHandler },
	{ CGIfile_appinit, AppInit_CGIHandler },
	{ CGIfile_reboot, AppReboot_CGIHandler }
#if (LOAD_FS_SDCARD==1)
	,
	{ CGIfile_sdfilelist, App_EN675sdfilelistCGIHandler },
	{ CGIfile_sdsetup, AppSDSetup_CGIHandler }
#endif
#if (JPEG_WEBVIEW==1)
	,
	{ CGIfile_still1, AppJpegStill_CGIHandler },
	{ CGIfile_still2, AppJpegStill_CGIHandler },
	{ CGIfile_still3, AppJpegStill_CGIHandler }
#endif
#if defined(__ISP__)
	,
	{ CGIfile_isp, App_ISPCGIHandler },
	{ CGIfile_ispky, App_ISPKeyCGIHandler }
#endif
#if defined(__WIFI__)
	,
	{ CGIfile_wifiap, App_WiFiAP_CGIHandler },
	{ CGIfile_wifista, App_WiFiSTA_CGIHandler }
#endif
};

//*****************************************************************************
//!
//! The number of individual CGI URIs that are configured for this system.
//!
//*****************************************************************************
#define NUM_CONFIG_CGI_URIS     (sizeof(g_psConfigCGIURIs) / sizeof(g_psConfigCGIURIs[0]))

//*****************************************************************************
//!
//! Initialize the CGI functionality by setting up pointers to
//! the arrays of tags and handlers.
//!
//*****************************************************************************
void cgi_init(void)
{
	// Pass our CGI handlers to the HTTP server.
	http_set_cgi_handlers(g_psConfigCGIURIs, NUM_CONFIG_CGI_URIS);
}

#endif
#endif
