#ifndef CGISSI_EN675_H_
#define CGISSI_EN675_H_

#define CGIfile_reboot	"/reboot.cgi"
#define CGIfile_query	"/appquery.cgi"
#define CGIfile_update	"/appupdate.cgi"
#define CGIfile_update2	"/appupdate2.cgi"
#define CGIfile_appinit	"/appinit.cgi"
#define CGIfile_still1	"/jpg/"
#define CGIfile_still2	"/cgi-bin/video.cgi"
#define CGIfile_still3	"/jpg/image.jpg"
#define CGIfile_sdsetup "/appsdsetup.cgi"
#define CGIfile_sdfilelist "/ls.html"

#define SSI_ENTR_state	100
#define SSI_ENTR_func	101
#define SSI_ENTR_tcp	102
#define SSI_ENTR_upnp	103
#define SSI_ENTR_vinfo	104
#define SSI_ENTR_sdformat 105
#define SSI_ENTR_user	106
#define SSI_ENTR_date	107
#define SSI_ENTR_video	108
#define SSI_ENTR_play	109
#define SSI_ENTR_info_m	110
#define SSI_ENTR_info_t 111
#define SSI_ENTR______1	112
#define SSI_ENTR_uping	113
#define SSI_ENTR_info_r 114

extern int App_EN675SSIHandler(int iIndex, char *pcInsert, int iInsertLen);

extern const char *App_EN675CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
extern const char *AppReboot_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
extern const char *AppUpdate_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
extern const char *AppUpdate2_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
extern const char *AppInit_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
extern const char *App_EN675sdfilelistCGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
extern const char *AppJpegStill_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
extern const char *AppSDSetup_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

#endif
