/* Copyright (c) 2013-18 Eyenix Corporation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. The name of Eyenix may not be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* 4. This software may only be redistributed and used in connection with an Eyenix
* product.
*
* THIS SOFTWARE IS PROVIDED BY EYENIX "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL EYENIX BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*
*/

#include "dev.h"

#if (ENX_RTSP_use==1)

#include "msg.h"
#include "user.h"

#if (RTSPD_AUTH_MODE==RTSPD_AUTH_BASIC)
#include "Base64.h"
#elif (RTSPD_AUTH_MODE==RTSPD_AUTH_DIGEST)
#include "md5.h"
#endif

#include "rtspd.h"
#include "rtspd_authorization.h"

#if 0
#define ENTER() printf(" IN %04d:%s\r\n", __LINE__, __func__)
#define LEAVE() printf("OUT %04d:%s\r\n", __LINE__, __func__)
#else
#define ENTER()
#define LEAVE()
#endif

#if (RTSPD_AUTH_MODE==RTSPD_AUTH_BASIC)
ENX_OKFAIL rtspd_client_rtsp_authorization(char *buf, rtsp_method_type method)
{
	ENTER();

	size_t nonce_len;
	ENX_OKFAIL nRes = ENX_FAIL;
	char *datacpy = &buf[sizeof(RTSPD_AUTHRIZATION_BASIC)-1];
	char *baseDecodeString = NULL;

	baseDecodeString = (char *)base64_decode(datacpy, strlen(datacpy), &nonce_len);
	if (baseDecodeString == NULL) {
		flprintf("Malloc fail\r\n");
	} else {
		char *id = baseDecodeString;
		char *colon = strchr(baseDecodeString, ':');
		char *pw = colon + 1;
		colon[0] = '\0';
		nRes = UserPasswordCheck(id, pw);

		vPortFree(baseDecodeString);
	}

	LEAVE();
	return nRes;

	UNUSED(method);
}
#elif (RTSPD_AUTH_MODE==RTSPD_AUTH_DIGEST)

#define ENX_RTSP_AUTH_strUSERNAME	"username"
#define ENX_RTSP_AUTH_strREALM		"realm"
#define ENX_RTSP_AUTH_strNONCE		"nonce"
#define ENX_RTSP_AUTH_strURI		"uri"
#define ENX_RTSP_AUTH_strRESPONSE	"response"
#define ENX_RTSP_AUTH_strALGORITHM	"algorithm"
#define ENX_RTSP_AUTH_strQOP		"qop"
#define ENX_RTSP_AUTH_strCNONCE		"cnonce"
#define ENX_RTSP_AUTH_strNC			"nc"
#define ENX_RTSP_AUTH_strOPAQUE		"opaque"

enum {
	ENX_RTSP_AUTH_numUSERNAME,	// 0
	ENX_RTSP_AUTH_numREALM,		// 1
	ENX_RTSP_AUTH_numNONCE,		// 2
	ENX_RTSP_AUTH_numURI,		// 3
	ENX_RTSP_AUTH_numRESPONSE,	// 4
	ENX_RTSP_AUTH_numALGORITHM,	// 5
	ENX_RTSP_AUTH_numQOP,		// 6
	ENX_RTSP_AUTH_numCNONCE,	// 7
	ENX_RTSP_AUTH_numNC,		// 8
	ENX_RTSP_AUTH_numOPAQUE,	// 9
	ENX_RTSP_AUTH_numNONE,
};

typedef struct {
	int checked;
	char *data;
} rtsp_auth_header;

typedef struct {
	char *strHeaderfield;
	rtsp_auth_header rahList[ENX_RTSP_AUTH_numNONE];
} rtsp_auth_headerlist;

static char *rtspd_client_rtsp_auth_del_quotes(char *temp)
{
	temp = strchr(temp, '=') + 1;
	if (temp) {
		char *temp2 = strchr(temp, '\"');
		if (temp2) {
			temp = temp2 + 1;
			temp2 = strchr(temp2 + 1, '\"');
			temp2[0] = '\0';
		}
	}
	return temp;
}

ENX_OKFAIL rtspd_client_rtsp_authorization(char *buf, rtsp_method_type method)
{
	ENX_OKFAIL nRes = ENX_FAIL;
	rtsp_auth_headerlist rahItem;
	memset(&rahItem, 0, sizeof(rtsp_auth_headerlist));

 	rahItem.strHeaderfield = (char *)pvPortMalloc((size_t)strlen(buf) + 1);
	if (rahItem.strHeaderfield == NULL) {
		flprintf("Malloc Fail\r\n");
	} else {
		strcpy(rahItem.strHeaderfield, buf);

		char *temp = NULL, *strHeader = strtok(rahItem.strHeaderfield, ",");
		while (strHeader != NULL) {
			if ((rahItem.rahList[ENX_RTSP_AUTH_numUSERNAME].checked == 0) && (temp = strstr(strHeader, ENX_RTSP_AUTH_strUSERNAME))) {
				rahItem.rahList[ENX_RTSP_AUTH_numUSERNAME].checked = 1;
				rahItem.rahList[ENX_RTSP_AUTH_numUSERNAME].data = rtspd_client_rtsp_auth_del_quotes(temp);
			} else if ((rahItem.rahList[ENX_RTSP_AUTH_numREALM].checked == 0) && (temp = strstr(strHeader, ENX_RTSP_AUTH_strREALM))) {
				rahItem.rahList[ENX_RTSP_AUTH_numREALM].checked = 1;
				rahItem.rahList[ENX_RTSP_AUTH_numREALM].data = rtspd_client_rtsp_auth_del_quotes(temp);
			} else if ((rahItem.rahList[ENX_RTSP_AUTH_numNONCE].checked == 0) && (temp = strstr(strHeader, ENX_RTSP_AUTH_strNONCE))) {
				rahItem.rahList[ENX_RTSP_AUTH_numNONCE].checked = 1;
				rahItem.rahList[ENX_RTSP_AUTH_numNONCE].data = rtspd_client_rtsp_auth_del_quotes(temp);
			} else if ((rahItem.rahList[ENX_RTSP_AUTH_numURI].checked == 0) && (temp = strstr(strHeader, ENX_RTSP_AUTH_strURI))) {
				rahItem.rahList[ENX_RTSP_AUTH_numURI].checked = 1;
				rahItem.rahList[ENX_RTSP_AUTH_numURI].data = rtspd_client_rtsp_auth_del_quotes(temp);
			} else if ((rahItem.rahList[ENX_RTSP_AUTH_numRESPONSE].checked == 0) && (temp = strstr(strHeader, ENX_RTSP_AUTH_strRESPONSE))) {
				rahItem.rahList[ENX_RTSP_AUTH_numRESPONSE].checked = 1;
				rahItem.rahList[ENX_RTSP_AUTH_numRESPONSE].data = rtspd_client_rtsp_auth_del_quotes(temp);
			} else if ((rahItem.rahList[ENX_RTSP_AUTH_numALGORITHM].checked == 0) && (temp = strstr(strHeader, ENX_RTSP_AUTH_strALGORITHM))) {
				rahItem.rahList[ENX_RTSP_AUTH_numALGORITHM].checked = 1;
				rahItem.rahList[ENX_RTSP_AUTH_numALGORITHM].data = rtspd_client_rtsp_auth_del_quotes(temp);
			} else if ((rahItem.rahList[ENX_RTSP_AUTH_numQOP].checked == 0) && (temp = strstr(strHeader, ENX_RTSP_AUTH_strQOP))) {
				rahItem.rahList[ENX_RTSP_AUTH_numQOP].checked = 1;
				rahItem.rahList[ENX_RTSP_AUTH_numQOP].data = rtspd_client_rtsp_auth_del_quotes(temp);
			} else if ((rahItem.rahList[ENX_RTSP_AUTH_numCNONCE].checked == 0) && (temp = strstr(strHeader, ENX_RTSP_AUTH_strCNONCE))) {
				rahItem.rahList[ENX_RTSP_AUTH_numCNONCE].checked = 1;
				rahItem.rahList[ENX_RTSP_AUTH_numCNONCE].data = rtspd_client_rtsp_auth_del_quotes(temp);
			} else if ((rahItem.rahList[ENX_RTSP_AUTH_numNC].checked == 0) && (temp = strstr(strHeader, ENX_RTSP_AUTH_strNC))) {
				rahItem.rahList[ENX_RTSP_AUTH_numNC].checked = 1;
				rahItem.rahList[ENX_RTSP_AUTH_numNC].data = rtspd_client_rtsp_auth_del_quotes(temp);
			} else if ((rahItem.rahList[ENX_RTSP_AUTH_numOPAQUE].checked == 0) && (temp = strstr(strHeader, ENX_RTSP_AUTH_strOPAQUE))) {
				rahItem.rahList[ENX_RTSP_AUTH_numOPAQUE].checked = 1;
				rahItem.rahList[ENX_RTSP_AUTH_numOPAQUE].data = rtspd_client_rtsp_auth_del_quotes(temp);
			} else {
				flprintf("unknown header [%s]\r\n", strHeader);
			}
			strHeader = strtok(NULL, ",");
		}

#if 0
		int i;
		for (i = 0; i < ENX_RTSP_AUTH_numNONE; i++) {
			if (i == ENX_RTSP_AUTH_numUSERNAME) printf("%10s", ENX_RTSP_AUTH_strUSERNAME);
			if (i == ENX_RTSP_AUTH_numREALM) printf("%10s", ENX_RTSP_AUTH_strREALM);
			if (i == ENX_RTSP_AUTH_numNONCE) printf("%10s", ENX_RTSP_AUTH_strNONCE);
			if (i == ENX_RTSP_AUTH_numURI) printf("%10s", ENX_RTSP_AUTH_strURI);
			if (i == ENX_RTSP_AUTH_numRESPONSE) printf("%10s", ENX_RTSP_AUTH_strRESPONSE);
			if (i == ENX_RTSP_AUTH_numALGORITHM) printf("%10s", ENX_RTSP_AUTH_strALGORITHM);
			if (i == ENX_RTSP_AUTH_numCNONCE) printf("%10s", ENX_RTSP_AUTH_strCNONCE);
			if (i == ENX_RTSP_AUTH_numNC) printf("%10s", ENX_RTSP_AUTH_strNC);
			if (i == ENX_RTSP_AUTH_numQOP) printf("%10s", ENX_RTSP_AUTH_strQOP);
			if (i == ENX_RTSP_AUTH_numOPAQUE) printf("%10s", ENX_RTSP_AUTH_strOPAQUE);
			printf(" Idx:%d Ck:%d Txt:[%s]\r\n", i, rahItem.rahList[i].checked, rahItem.rahList[i].data);
		}
#endif

		char HA1[33] = {0}, HA2[33] = {0}, HRes[33] = {0}; // 32 + 1(NULL)
		BYTE hash[16] = {0};
		do {
			// Hash A1 = hash(id:REALM:pw)
			memcpy(HA1, (void *)gtNetwork.hashUserPw, 32);

			// Hash A2 = hash(METHOD:uri)
			MD5_CTX mdStep2;
			MD5Init(&mdStep2);
			switch (method) {
				case ENX_RTSP_METHOD_OPTIONS:
					MD5Update(&mdStep2, (unsigned char *)ENX_RTSP_METHOD_strOPTIONS, sizeof(ENX_RTSP_METHOD_strOPTIONS) - 1);
					break;
				case ENX_RTSP_METHOD_DESCRIBE:
					MD5Update(&mdStep2, (unsigned char *)ENX_RTSP_METHOD_strDESCRIBE, sizeof(ENX_RTSP_METHOD_strDESCRIBE) - 1);
					break;
				case ENX_RTSP_METHOD_SETUP:
					MD5Update(&mdStep2, (unsigned char *)ENX_RTSP_METHOD_strSETUP, sizeof(ENX_RTSP_METHOD_strSETUP) - 1);
					break;
				case ENX_RTSP_METHOD_PLAY:
					MD5Update(&mdStep2, (unsigned char *)ENX_RTSP_METHOD_strPLAY, sizeof(ENX_RTSP_METHOD_strPLAY) - 1);
					break;
				case ENX_RTSP_METHOD_PAUSE:
					MD5Update(&mdStep2, (unsigned char *)ENX_RTSP_METHOD_strPAUSE, sizeof(ENX_RTSP_METHOD_strPAUSE) - 1);
					break;
				case ENX_RTSP_METHOD_TEARDOWN:
					MD5Update(&mdStep2, (unsigned char *)ENX_RTSP_METHOD_strTEARDOWN, sizeof(ENX_RTSP_METHOD_strTEARDOWN) - 1);
					break;
				case ENX_RTSP_METHOD_GET_PARAMETER:
					MD5Update(&mdStep2, (unsigned char *)ENX_RTSP_METHOD_strGET_PARAMETER, sizeof(ENX_RTSP_METHOD_strGET_PARAMETER) - 1);
					break;
				case ENX_RTSP_METHOD_SET_PARAMETER:
					MD5Update(&mdStep2, (unsigned char *)ENX_RTSP_METHOD_steSET_PARAMETER, sizeof(ENX_RTSP_METHOD_steSET_PARAMETER) - 1);
					break;
			}
			MD5Update(&mdStep2, (unsigned char *)":", 1);
			MD5Update(&mdStep2, (unsigned char *)rahItem.rahList[ENX_RTSP_AUTH_numURI].data, strlen(rahItem.rahList[ENX_RTSP_AUTH_numURI].data));
			MD5Final(hash, &mdStep2);
			Hex2Str32B(mdStep2.digest, HA2);

			// Hash Response = hash(Hash A1:NONCE:NC:CNONCE:QOP:Hash A2)
			MD5_CTX mdStep3;
			MD5Init(&mdStep3);
			MD5Update(&mdStep3, (unsigned char *)HA1, sizeof(HA1) - 1);
			MD5Update(&mdStep3, (unsigned char *)":", 1);
			MD5Update(&mdStep3, (unsigned char *)rahItem.rahList[ENX_RTSP_AUTH_numNONCE].data, strlen(rahItem.rahList[ENX_RTSP_AUTH_numNONCE].data));
			MD5Update(&mdStep3, (unsigned char *)":", 1);
			MD5Update(&mdStep3, (unsigned char *)rahItem.rahList[ENX_RTSP_AUTH_numNC].data, strlen(rahItem.rahList[ENX_RTSP_AUTH_numNC].data));
			MD5Update(&mdStep3, (unsigned char *)":", 1);
			MD5Update(&mdStep3, (unsigned char *)rahItem.rahList[ENX_RTSP_AUTH_numCNONCE].data, strlen(rahItem.rahList[ENX_RTSP_AUTH_numCNONCE].data));
			MD5Update(&mdStep3, (unsigned char *)":", 1);
			MD5Update(&mdStep3, (unsigned char *)rahItem.rahList[ENX_RTSP_AUTH_numQOP].data, strlen(rahItem.rahList[ENX_RTSP_AUTH_numQOP].data));
			MD5Update(&mdStep3, (unsigned char *)":", 1);
			MD5Update(&mdStep3, (unsigned char *)HA2, sizeof(HA2) - 1);
			MD5Final(hash, &mdStep3);
			Hex2Str32B(mdStep3.digest, HRes);
		} while(0);

		if (strcmp(HRes, rahItem.rahList[ENX_RTSP_AUTH_numRESPONSE].data) == 0) {
			nRes = ENX_OK;
		}

		vPortFree(rahItem.strHeaderfield);
	}

	LEAVE();
	return nRes;
}
#endif
#endif
